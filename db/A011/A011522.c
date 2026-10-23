/*
 * A011522  Stirling numbers of first kind S1(12,n).
 * https://oeis.org/A011522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = s(12,n) – signed Stirling number of the first kind. For n
 *            > 12 the value is 0
 * Range:     i8 none, i16 none, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n > 12) return 0;

    static seq_t s[13][13];
    static bool init = false;

    if (!init) {
        /* initialize the table s[i][j] = s(i,j) for 0 ≤ i ≤ 12, 0 ≤ j ≤ i */
        for (idx_t i = 0; i <= 12; ++i) {
            for (idx_t j = 0; j <= 12; ++j) {
                s[i][j] = 0;
            }
        }
        s[0][0] = 1;
        for (idx_t i = 1; i <= 12; ++i) {
            for (idx_t j = 1; j <= i; ++j) {
                s[i][j] = s[i - 1][j - 1]
                          - (seq_t)(i - 1) * s[i - 1][j];
            }
        }
        init = true;
    }

    return s[12][n];
}
