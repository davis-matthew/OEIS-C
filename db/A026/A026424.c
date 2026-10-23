/*
 * A026424  Number of prime divisors (counted with multiplicity) is odd;
 *          Liouville function lambda(n) (A008836) is negative.
 * https://oeis.org/A026424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A026424: positive integers n ≥ 2 whose Ω(n) is odd
   (i.e. the Liouville function λ(n)=(-1)^Ω(n) is negative).
   The sequence is generated in increasing order and cached so that
   calls to a(n) may appear in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------  Prime table  ---------- */

static uint64_t *primes      = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;
static uint64_t prime_limit  = 0;

/* Ensure all primes ≤ limit are in the table. */
static void generate_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit) return;

    if (prime_cap < limit) {
        size_t new_cap = prime_cap ? prime_cap * 2 : limit;
        primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }

    for (uint64_t cand = prime_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) primes[prime_cnt++] = cand;
    }
    prime_limit = limit;
}

/* Smallest prime factor of n (n ≥ 2). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    if (n % 2 == 0) return 2;
    generate_primes_up_to(n);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return p;
    }
    return n;               /* n itself is prime */
}

/* ---------  Check for odd Ω(n)  --------- */

static bool odd_omega(uint64_t n)
{
    bool parity = false;          /* false = even, true = odd */
    while (n > 1) {
        n /= smallest_prime_factor(n);
        parity = !parity;
    }
    return parity;
}

/* ---------  Sequence cache  --------- */

static seq_t *seq_cache      = NULL;   /* cached values */
static size_t seq_len         = 0;     /* number of cached terms */
static size_t seq_cap         = 0;     /* capacity of cache array */
static uint64_t next_candidate = 2;    /* next integer to test */

/* Allocate more space for seq_cache if needed. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ---------  Exported function  --------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for 0, safe default */

    /* Extend the sequence until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        if (odd_omega(next_candidate)) {
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
