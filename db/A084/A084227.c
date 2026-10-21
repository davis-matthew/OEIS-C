/*
 * A084227  Numbers of the form p*q^k with distinct primes p and q, k>0.
 * https://oeis.org/A084227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084227: numbers of the form p*q^k with distinct primes p and q, k>0.
   The algorithm generates the sequence in ascending order by testing each
   integer for having exactly two distinct prime factors, at least one of
   whose exponent equals 1.  The sequence is cached so any n >= 1 can be
   answered in O(1) after the initial precomputation.  For n up to about
   10 000 the 10 000th term is below 5×10^5, so a simple sieve up to
   2 million suffices for prime generation.  This satisfies the
   requirements for both C11 and C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- prime generation ---------- */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void init_primes(void)
{
    const uint64_t LIMIT = 2000000ULL;            /* enough for n≈10000 */
    size_t est = (size_t)(LIMIT / log((double)LIMIT) + 10);
    primes_cap = est;
    primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) return;   /* allocation failure not handled */

    uint8_t *sieve = (uint8_t*)calloc(LIMIT + 1, 1);
    if (!sieve) return;

    uint32_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            if (idx == primes_cap) {        /* extend array if needed */
                primes_cap *= 2;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[idx++] = i;
            if (i * i > LIMIT) continue;
            for (uint64_t j = i * i; j <= LIMIT; j += i) sieve[j] = 1;
        }
    }
    primes_len = idx;
    free(sieve);
}

/* ---------- test if n has the required form ---------- */
static bool is_form_uint64(uint64_t n)
{
    uint64_t temp = n;
    int distinct = 0;
    int exp1 = 0, exp2 = 0;

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            ++distinct;
            if (distinct > 2) return false;

            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }

            if (distinct == 1) exp1 = e;
            else exp2 = e;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        ++distinct;
        if (distinct > 2) return false;
        if (distinct == 1) exp1 = 1;
        else exp2 = 1;
    }

    if (distinct != 2) return false;
    return (exp1 == 1 || exp2 == 1);
}

/* ---------- sequence storage ---------- */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t current = 2;            /* next integer to test */

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    static bool primes_inited = false;
    if (!primes_inited) { init_primes(); primes_inited = true; }

    /* generate terms until we have enough */
    while (seq_len < (size_t)n) {
        while (true) {
            if (is_form_uint64(current)) {
                if (seq_len == seq_cap) {      /* grow the array */
                    seq_cap = seq_cap ? seq_cap * 2 : 64;
                    seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
                }
                seq_arr[seq_len++] = (seq_t)current;
                ++current;
                break;
            }
            ++current;
        }
    }
    return seq_arr[n - 1];
}
