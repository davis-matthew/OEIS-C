/*
 * A142749  Primes congruent to 22 mod 59.
 * https://oeis.org/A142749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 22 modulo 59
 * a(n) = the n-th prime p such that p % 59 == 22   (offset: 1)
 * The implementation builds primes up to a growing limit
 * until enough AP‑primes have been collected to satisfy the
 * requested index.  It relies on the helper function
 * build_primes_upto(limit, &primes, &cnt) from ../utils/build_primes_upto.h.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"

static uint64_t *s_primes = NULL;   /* array of primes produced by the sieve   */
static size_t   s_cnt   = 0;        /* number of primes in the array           */
static idx_t    s_limit = 0;        /* the current upper bound of the sieve    */

/* Ensure the sieve has produced primes up to new_limit.
 * The previous prime array is freed before allocating the new one. */
static void
grow_primes(idx_t new_limit)
{
    free(s_primes);
    build_primes_upto(new_limit, &s_primes, &s_cnt);
    s_limit = new_limit;
}

/* Return the n-th prime congruent to 22 mod 59 (n >= 1). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* for safety – offset is 1 */

    /* Initial sieve if this is the first call. */
    if (s_primes == NULL) {
        idx_t limit = n * 60 + 100;   /* rough upper bound estimate   */
        if (limit < 1000) limit = 1000;
        grow_primes(limit);
    }

    /* Grow the sieve until enough primes in the desired progression are found. */
    while (1) {
        idx_t count = 0;
        for (size_t i = 0; i < s_cnt; ++i)
            if (s_primes[i] % 59ULL == 22ULL)
                ++count;

        if (count >= n) break;                 /* sufficient primes built */
        grow_primes(s_limit * 2);               /* double the bound and rerun */
    }

    /* Locate the n-th such prime. */
    idx_t  found = 0;
    seq_t result = 0;
    for (size_t i = 0; i < s_cnt; ++i) {
        if (s_primes[i] % 59ULL == 22ULL) {
            ++found;
            if (found == n) {
                result = (seq_t)s_primes[i];
                break;
            }
        }
    }
    return result;
}
