/*
 * A011520  Stirling numbers of first kind S1(10,n).
 * https://oeis.org/A011520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = s(10,n) (Stirling numbers of the first kind)
 * Range:     i8 none, i16 none, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t s[11][11];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i <= 10; ++i) {
            s[i][0] = 0;
            s[0][i] = 0;
        }
        s[0][0] = 1;
        for (idx_t i = 1; i <= 10; ++i) {
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = (seq_t)(i - 1) * s[i - 1][k];
                s[i][k] = s[i - 1][k - 1] - term;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > 10)
        return 0;

    return s[10][n];
}
