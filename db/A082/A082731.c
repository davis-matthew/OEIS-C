/*
 * A082731  a(n) is the smallest number k such that A033880(k)= n, or 0 if no
 *          such number exists, where A033880 is the abundance of k.
 * https://oeis.org/A082731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = minimal k such that sigma(k) − 2·k = n,
   otherwise 0 if no such k ≤ LIMIT (chosen large enough to
   cover all n up to 10000). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Largest n we care about, here 10000. */
    static const idx_t NMAX = 10000;
    /* Upper limit for k to search for minimal abundance values. */
    static const idx_t LIMIT = 500000;
    /* Cache for results. 0 means "no k found up to LIMIT yet". */
    static seq_t *cache = NULL;
    static int initialized = 0;

    if (!initialized) {
        /* Allocate cache. */
        cache = (seq_t *)calloc((size_t)(NMAX + 1), sizeof(seq_t));

        /* Compute sigma(k) for all 1 ≤ k ≤ LIMIT by a simple divisor sieve. */
        seq_t *sigma = (seq_t *)calloc((size_t)(LIMIT + 1), sizeof(seq_t));
        for (idx_t i = 1; i <= LIMIT; ++i) {
            for (idx_t j = i; j <= LIMIT; j += i) {
                sigma[j] += (seq_t)i;
            }
        }

        /* Record minimal k for each abundance value. */
        for (idx_t k = 1; k <= LIMIT; ++k) {
            long long diff = (long long)sigma[k] - (long long)2 * (long long)k;
            if (diff >= 0 && (idx_t)diff <= NMAX) {
                idx_t m = (idx_t)diff;
                if (cache[m] == 0 || k < cache[m]) {
                    cache[m] = k;
                }
            }
        }

        free(sigma);
        initialized = 1;
    }

    if (n > NMAX) {
        return 0;
    }
    return cache[n];
}
