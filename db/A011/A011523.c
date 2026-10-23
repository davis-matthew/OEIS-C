/*
 * A011523  Stirling numbers of first kind S1(13,n).
 * https://oeis.org/A011523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 none, i16 none, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = (-1)^(n-1) * S1(13,n)
 *            (unsigned Stirling numbers of the first kind)
 *  with a(n)=0 for n<1 or n>13.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t value[14] = {0};

    if (!init) {
        const idx_t N = (idx_t)13;
        /* unsigned Stirling numbers s(i,j), 0 ≤ i,j ≤ 13 */
        static seq_t s[14][14];
        for (idx_t i = 0; i <= N; ++i) {
            for (idx_t j = 0; j <= N; ++j) s[i][j] = 0;
        }
        s[0][0] = 1;                     /* s(0,0)=1 */

        /* compute s(i,j) via recurrence */
        for (idx_t i = 1; i <= N; ++i) {
            for (idx_t j = 1; j <= i; ++j) {
                /* s(i,j) = s(i-1,j-1) + (i-1)*s(i-1,j) */
                s[i][j] = s[i-1][j-1] + (seq_t)(i-1) * s[i-1][j];
            }
        }

        /* apply sign and store values for 1..13 */
        for (idx_t k = 1; k <= N; ++k) {
            seq_t sign = ((((k-1) & 1) == 0) ? 1 : -1);   /* (-1)^(k-1) */
            value[(idx_t)k] = sign * s[N][k];
        }
        init = true;
    }

    if (n >= 1 && n <= (idx_t)13) {
        return value[(idx_t)n];
    }
    return 0;
}
