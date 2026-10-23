/*
 * A165138  Smallest prime p > prime(n) such that p+prime(n) is a semiprime.
 * https://oeis.org/A165138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A165138
 *  a(n) = smallest prime p > prime(n) such that p + prime(n) is a semiprime.
 *  The implementation builds a list of primes on demand and tests
 *  semiprimality by trial division with successive primes.
 *  It is correct for all n ≥ 1 and works with any seq_t size
 *  (8‑128 bits or arbitrary precision).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t) */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t) */

/* ---------- prime list management ---------- */
static uint64_t *g_prime = NULL;  /* array of primes, 0‑based */
static size_t   g_prime_cnt = 0;  /* number of primes stored */

/* generate primes until we have at least 'need' primes (1‑based) */
static void
ensure_primes(size_t need)
{
    if (g_prime_cnt >= need)
        return;

    size_t cap = g_prime_cnt ? g_prime_cnt * 2 : 16;
    while (cap < need)
        cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(g_prime, cap * sizeof(uint64_t));
    if (!tmp)
        abort();      /* out of memory */
    g_prime = tmp;

    uint64_t cand = g_prime_cnt ? g_prime[g_prime_cnt - 1] + 1 : 2;
    while (g_prime_cnt < need) {
        if (is_prime_small((seq_t)cand))
            g_prime[g_prime_cnt++] = cand;
        ++cand;
    }
}

/* ---------- semiprime test ---------- */
static bool
is_semiprime(seq_t x)
{
    if (x < 4)          /* smallest semiprime is 4 = 2*2 */
        return false;

    seq_t tmp = x;
    int  factors = 0;
    uint64_t p = 2;                       /* next prime to try */

    while ((seq_t)p * (seq_t)p <= tmp) {  /* loop while p^2 <= tmp */
        seq_t p_seq = (seq_t)p;
        if (tmp % p_seq == 0) {
            while (tmp % p_seq == 0) {
                tmp /= p_seq;
                ++factors;
                if (factors > 2)
                    return false;
            }
        }
        /* advance to the next prime */
        p = (p == 2) ? 3 : next_prime(p);
    }

    if (tmp > 1)            /* remaining prime factor */
        ++factors;

    return factors == 2;
}

/* ---------- main sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, but return 0 */

    /* need the first n primes to get prime(n) */
    ensure_primes((size_t)n);

    seq_t prime_n = (seq_t)g_prime[n - 1];

    /* search for the first prime p > prime_n such that p+prime_n is semiprime */
    size_t cand_idx = n;                 /* index of the next prime to test (0‑based) */

    for (;; ++cand_idx) {
        ensure_primes(cand_idx + 1);      /* make sure the prime exists */

        seq_t p   = (seq_t)g_prime[cand_idx];
        seq_t sum = p + prime_n;

        if (is_semiprime(sum))
            return p;
    }
}
