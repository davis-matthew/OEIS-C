/*
 * A162923  Numbers k such that the equation sigma(sigma(x)) = k has no
 *          solutions.
 * https://oeis.org/A162923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers not of the form sigma(sigma(x)) for any x >= 1
 *            Implementation: compute sigma values up to a bound M, mark all
 *            values t = sigma(sigma(i)) that are <= M, then scan for the n-th
 *            unmarked integer. The bound M is increased until enough missing
 *            numbers are found. This works for the first...
 * Range:     u8 n <= 11, u16 n <= 5166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* not expected, but safe */

    /* initial bound: about 4*n to be generous */
    idx_t M = n * 4 + 100;
    if (M < 10) M = 10;           /* at least a small size */

    while (1) {
        /* allocate sigma array */
        seq_t *sigma = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
        if (!sigma) return 0;      /* allocation failed */

        /* allocate seen array */
        char *seen = (char *)calloc((size_t)(M + 1), sizeof(char));
        if (!seen) { free(sigma); return 0; }

        /* compute sigma(i) for 1 <= i <= M */
        for (idx_t i = 1; i <= M; ++i) {
            for (idx_t j = i; j <= M; j += i) {
                sigma[j] += (seq_t)i;
            }
        }

        /* mark all t = sigma(sigma(i)) that are within the bound */
        for (idx_t i = 1; i <= M; ++i) {
            seq_t s = sigma[i];
            if (s <= M) {
                seq_t t = sigma[(size_t)s];
                if (t <= M) {
                    seen[(size_t)t] = 1;
                }
            }
        }

        /* find the n-th unmarked integer */
        idx_t count = 0;
        seq_t result = 0;
        for (idx_t k = 1; k <= M; ++k) {
            if (!seen[(size_t)k]) {
                ++count;
                if (count == n) {
                    result = (seq_t)k;
                    break;
                }
            }
        }

        /* clean up */
        free(seen);
        free(sigma);

        if (result != 0) return result;   /* success */

        /* not enough, increase the bound and retry */
        if (M > SIZE_MAX / 2) return 0;    /* avoid overflow */
        M *= 2;
    }
}
