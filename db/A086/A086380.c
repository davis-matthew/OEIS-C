/*
 * A086380  Numbers n such that p = n^2 + 2, p+2 and p+6 are consecutive
 *          primes.
 * https://oeis.org/A086380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A086380:
 *   a(n) = the n‑th integer m such that
 *          p = m²+2, p+2, p+6 are primes and there is no other
 *          prime between p+2 and p+6.
 *   The sequence starts 3, 15, 33, 57, …
 *   The function returns the n‑th such integer, computing values on
 *   demand and memoizing past results.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

/* Small prime divisors used for a quick compositeness test. */
static const unsigned int small_primes[] = {
    3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,
    61,67,71,73,79,83,89,97,101,103,107,109,113,127,
    131,137,139,149,151,157,163,167,173,179,181,191,
    193,197,199,211,223,227,229,233,239,241,251,257,
    263,269,271,277,281,283,293,307,311,313,317,331,
    337,347,349,353,359,367,373,379,383,389,397,401,
    409,419,421,431,433,439,443,449,457,461,463,467,
    479,487,491,499,503,509,521,523,541
};
static const size_t num_small_primes = sizeof(small_primes)/sizeof(small_primes[0]);

/* Test whether x is composite using small prime divisors. */
static bool is_composite_small(const seq_t *x)
{
    for (size_t i = 0; i < num_small_primes; ++i) {
        unsigned int p = small_primes[i];
        if (*x % p == 0) return (*x != (seq_t)p);
    }
    return false;
}

/* Returns true if x is prime, using small prime test as a quick filter. */
static bool is_prime(const seq_t *x)
{
    if (is_composite_small(x)) return false;
    return isPrimeGeneric(*x);
}

/* Returns true if x is composite, using small prime test first. */
static bool is_composite(const seq_t *x)
{
    if (is_composite_small(x)) return true;
    return !isPrimeGeneric(*x);
}

/* Cache of computed terms. */
static seq_t *cache    = NULL;  /* cached sequence values */
static size_t cache_len = 0;    /* number of cached terms   */
static size_t cache_cap = 0;    /* allocated capacity        */
static uint64_t next_candidate = 3; /* next n ≡ 3 (mod 6) to test */

/* Grow the cache array. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Scan candidates until we have at least `target` terms collected. */
static void scan_to_n(idx_t target)
{
    while (cache_len < target) {
        /* Candidate n (must be 3 mod 6) */
        uint64_t n = next_candidate;
        next_candidate += 6;

        /* Compute p = n² + 2 and related numbers */
        seq_t n_seq = (seq_t)n;
        seq_t p    = n_seq * n_seq + (seq_t)2; /* p = n^2 + 2 */
        seq_t p2   = p + (seq_t)2;             /* p+2 */
        seq_t p6   = p + (seq_t)6;             /* p+6 */
        seq_t p3   = p + (seq_t)3;             /* p+3 */
        seq_t p5   = p + (seq_t)5;             /* p+5 */

        /* Check the five numbers */
        if (!is_prime(&p))   continue;
        if (!is_prime(&p2))  continue;
        if (!is_prime(&p6))  continue;
        if (!is_composite(&p3)) continue;
        if (!is_composite(&p5)) continue;

        /* All checks passed – store the candidate */
        if (cache_len == cache_cap)
            grow_cache();

        cache[cache_len++] = n_seq;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n=0; safe return */
    scan_to_n(n);                    /* ensure at least n terms computed */
    return cache[n - 1];
}
