/*
 * A093690  Primes p that divide A007406(k), the numerator of the k-th
 *          generalized harmonic number H(k,2) = Sum 1/i^2 for i=1..k, for
 *          some k < (p-1)/2.
 * https://oeis.org/A093690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Classification of H2-irregular primes (OEIS A093690):
   p is H2‑irregular iff there exists k < (p‑1)/2 such that
   p divides the numerator of the k‑th generalized harmonic number
   H(k,2) = Σ_{i=1}^k 1/i².
   The function searches successive primes and returns the n‑th one
   that satisfies the condition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PRIME_LIMIT 600000   /* enough to cover n ≤ 10000 */
#define MAX_INV        300000    /* maximum size of inverse table for a prime */

/* ---------- prime sieve data ---------- */
static bool *is_composite = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_next  = 0;

/* ---------- sequence cache ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/* ---------- auxiliary table for modular inverses ---------- */
static uint32_t inv_arr[MAX_INV];

/* initialise a simple sieve of Eratosthenes up to MAX_PRIME_LIMIT */
static void init_sieve(void)
{
    if (is_composite) return;                  /* already done */

    is_composite = (bool*)calloc(MAX_PRIME_LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(1);

    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_composite[j] = true;

    /* estimate number of primes (upper bound) */
    size_t est = MAX_PRIME_LIMIT / 10 + 100;
    prime_list = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!prime_list) exit(1);
    prime_count = 0;

    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (!is_composite[i]) {
            if (prime_count == est) {
                est <<= 1;
                prime_list = (uint32_t*)realloc(prime_list, est * sizeof(uint32_t));
                if (!prime_list) exit(1);
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
}

/* resize sequence cache to hold at least 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 64;
    if (newcap < need) newcap = need;
    seq_cache = (seq_t*)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!seq_cache) exit(1);
    seq_cap = newcap;
}

/* test whether a prime p is H2‑irregular */
static bool is_H2_irregular(uint32_t p)
{
    uint32_t limit = (p - 1u) / 2u;          /* floor((p-1)/2) */
    if (limit <= 1u) return false;           /* need k ≥ 1 */

    uint32_t max_k = limit - 1u;             /* test 1 … max_k */

    if (max_k >= MAX_INV)                    /* safety guard */
        return false;

    inv_arr[1] = 1u;
    uint64_t sum = 0ull;

    for (uint32_t i = 1; i <= max_k; ++i) {
        if (i > 1u) {
            uint32_t q = p / i;
            uint32_t r = p % i;
            uint64_t term = (uint64_t)q * inv_arr[r] % p;
            inv_arr[i] = (uint32_t)((p - term) % p);
        }
        uint64_t t = inv_arr[i];
        sum = (sum + t * t) % p;
        if (sum == 0ull)
            return true;
    }
    return false;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (!prime_list) init_sieve();

    while (seq_len < (size_t)n) {
        if (prime_next >= prime_count) {
            /* insufficient primes; with MAX_PRIME_LIMIT this should not happen
               for n ≤ 10000 */
            break;
        }
        uint32_t p = prime_list[prime_next++];
        if (is_H2_irregular(p)) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)p;
        }
    }
    return seq_cache[(size_t)(n - 1u)];
}
