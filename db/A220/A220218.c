/*
 * A220218  Numbers where all exponents in its prime factorization are one
 *          less than a prime.
 * https://oeis.org/A220218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS Sequence A220218
 *
 *   a(n) = the n-th positive integer whose prime factorisation
 *          has every exponent equal to one less than a prime.
 *
 *   Method:  Enumerate the positive integers, test each for the
 *           property, and cache the ones that satisfy it.  The
 *           sequence is stored in increasing order, so each call
 *           to a(n) can be answered immediately from the cache.
 *   Complexity:   O(n * sqrt(a(n))) – for n ≤ 10 000 this is well
 *                 below 2 s on any modern machine.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of function prototypes provided by the
 * build utilities.  No direct use of these prototypes is needed
 * for this implementation. */

/* Test whether an integer value is prime (small integers only). */
static inline bool is_prime_int(int n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Check whether a given integer satisfies the exponent condition. */
static bool has_prime_minus_one_exponents(uint64_t x)
{
    uint64_t tmp = x;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            /* Exponent e must satisfy e+1 being prime. */
            if (!is_prime_int(e + 1))
                return false;
        }
    }
    if (tmp > 1) {            /* Remaining prime factor has exponent 1. */
        if (!is_prime_int(1 + 1))   /* 2 is always prime, but keep test. */
            return false;
    }
    return true;
}

/* Static cache of the sequence.  The cache grows only as needed
 * to answer requests.  All accesses are thread‑unsafe but
 * the test harness uses a single thread. */
static seq_t   *cache_seq   = NULL;   /* array of a(k) values */
static idx_t    cache_len   = 0;      /* number of values stored */
static uint64_t last_checked = 0;     /* largest integer already tested */

/* Public function as required by the test harness.  It returns
 * the n-th term of G05 (OEIS A220218) with OEIS indexing: a(1) is
 * the first element of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)  /* safety for out‑of‑bounds caller */
        return (seq_t)0;

    /* Grow the cache until we have at least n terms. */
    if (n > cache_len) {
        uint64_t cand = last_checked + 1;
        /* Allocate space for the initial cache if empty. */
        if (cache_seq == NULL) {
            cache_seq = (seq_t *)malloc(1 * sizeof(seq_t));
            if (!cache_seq) abort(); /* allocation failure */
        }

        while (cache_len < n) {
            if (has_prime_minus_one_exponents(cand)) {
                /* Append the new term, expanding the array if needed. */
                seq_t *tmp = (seq_t *)realloc(cache_seq,
                                              (cache_len + 1) * sizeof(seq_t));
                if (!tmp) abort();         /* allocation failure */
                cache_seq = tmp;
                cache_seq[cache_len] = (seq_t) cand;
                cache_len++;
            }
            cand++;                      /* move to the next integer */
        }
        last_checked = cand - 1;          /* cand has been incremented once more */
    }

    /* Return the requested element (OEIS indices start at 1). */
    return cache_seq[n - 1];
}
