/*
 * A011524  Stirling numbers of first kind S1(14,n).
 * https://oeis.org/A011524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = (-1)^(14-n) * |s(14,n)|, where |s(14,n)| are the unsigned
 *            Stirling numbers of the first kind. This implements the signed
 *            Stirling numbers s(14,n) (OEIS A011524). Values for n > 14 are
 *            defined as 0
 * Range:     i8 none, i16 none, i32 none, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the unsigned Stirling numbers s(14,k) once. */
    static seq_t s[15];
    static bool done = false;

    if (!done) {
        seq_t prev[15] = {0};
        seq_t curr[15] = {0};

        prev[0] = 1;                     /* s(0,0) = 1 */
        for (idx_t m = 1; m <= 14; ++m) {
            curr[0] = 0;
            for (idx_t k = 1; k <= m; ++k) {
                curr[k] = prev[k-1] + (seq_t)(m-1) * prev[k];
            }
            /* copy curr -> prev for next iteration */
            for (idx_t k = 0; k <= m; ++k) {
                prev[k] = curr[k];
            }
        }
        /* Save the 14th row (values for 1..14). */
        for (idx_t k = 1; k <= 14; ++k) {
            s[k] = prev[k];
        }
        done = true;
    }

    if (n < 1 || n > 14) {
        return 0;          /* s(14,n) = 0 for |n-14| > 0 */
    }

    /* Sign = (-1)^(14-n). */
    seq_t sign = (( (seq_t)(14 - n) ) & 1) ? (seq_t)-1 : (seq_t)1;
    return sign * s[(size_t)n];
}
