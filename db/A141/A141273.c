/*
 * A141273  Conjectured to be complete list of numbers not of the form 4*x^2 +
 *          4*y^2 + triangular number.
 * https://oeis.org/A141273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A141273: numbers not of the form 4*x^2 + 4*y^2 + T_k,
   where T_k = k(k+1)/2 (triangular numbers).  Returns a(n) for any n >= 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* external types defined by the build system */
extern seq_t a(idx_t n);  /* declaration for completeness */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n=0, return 0 as a sentinel */

    /* Initial estimate for the search limit.  This grows if needed. */
    idx_t limit = (n * 200 > 1200) ? n * 200 : 1200;

    while (1) {
        /* Allocate boolean array for representable numbers up to 'limit'. */
        bool *rep = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!rep)
            return 0;  /* allocation failure */
        memset(rep, 0, (limit + 1) * sizeof(bool));

        /* Allocate array for triangular numbers (upper bound limit+1).  */
        idx_t *tri = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
        if (!tri) {
            free(rep);
            return 0;  /* allocation failure */
        }

        /* Generate triangular numbers up to 'limit'. */
        idx_t tcnt = 0;
        for (idx_t k = 0; ; ++k) {
            idx_t T = k * (k + 1) / 2;
            if (T > limit)
                break;
            tri[tcnt++] = T;
        }

        /* Mark all representable numbers of the form 4x^2+4y^2+T_k. */
        for (idx_t x = 0; ; ++x) {
            idx_t base_x = 4 * x * x;
            if (base_x > limit)
                break;
            for (idx_t y = 0; ; ++y) {
                idx_t base_xy = base_x + 4 * y * y;
                if (base_xy > limit)
                    break;
                for (idx_t j = 0; j < tcnt; ++j) {
                    idx_t val = base_xy + tri[j];
                    if (val > limit)
                        break;
                    rep[val] = true;
                }
            }
        }

        /* Count non-representable numbers until we reach the target. */
        idx_t cnt = 0;
        seq_t result = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!rep[i]) {
                ++cnt;
                if (cnt == n) {
                    result = (seq_t)i;
                    break;
                }
            }
        }

        /* Clean up memory before possibly continuing. */
        free(rep);
        free(tri);

        if (cnt >= n)
            return result;  /* found the nth non-representable number */

        /* Increase the search limit and try again. */
        limit *= 2;
    }
}
