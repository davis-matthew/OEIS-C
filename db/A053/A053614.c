/*
 * A053614  Numbers that are not the sum of distinct triangular numbers.
 * https://oeis.org/A053614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A053614
 *  Numbers that are not the sum of distinct triangular numbers.
 *  The function computes the n‑th term (offset 1) by generating all
 *  sums of distinct triangular numbers up to a growing bound until the
 *  requested term is found.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* External integer types (supplied by the build) */
 /* seq_t : unsigned integer or arbitrary‑precision integer         */
 /* idx_t : unsigned 64‑bit integer (uint64_t)                      */

seq_t a(idx_t n)
{
    if (n == 0)                     /* safety for offset 1 */
        return (seq_t)0;

    idx_t bound = 1;                /* initial search bound */

    for (;;) {
        /* Allocate DP array of booleans (bytes) */
        unsigned char *dp =
            (unsigned char *)malloc((size_t)(bound + 1) * sizeof(unsigned char));
        if (!dp) abort();
        memset(dp, 0, (size_t)(bound + 1));
        dp[0] = 1;                 /* 0 is representable by empty sum */

        /* Consider distinct triangular numbers up to 'bound' */
        for (idx_t k = 1; ; ++k) {
            idx_t tri = (k * (k + 1)) / 2;   /* k-th triangular number */
            if (tri > bound) break;
            for (idx_t s = bound; s >= tri; --s) {
                if (dp[s - tri]) dp[s] = 1;
            }
        }

        /* Scan for the n-th non‑representable number */
        idx_t count = 0;
        for (idx_t i = 1; i <= bound; ++i) {
            if (!dp[i]) {
                ++count;
                if (count == n) {
                    seq_t result = (seq_t)i;
                    free(dp);
                    return result;
                }
            }
        }

        free(dp);                  /* not enough terms – grow bound */
        bound *= 2;
    }
}
