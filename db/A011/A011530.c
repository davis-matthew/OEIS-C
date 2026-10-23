/*
 * A011530  Stirling numbers of first kind S1(20,n).
 * https://oeis.org/A011530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = signed Stirling number of the first kind s(20, n)
 * Range:     i8 none, i16 none, i32 none, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static tables for s(0..20, 0..20) */
    static seq_t s[21][21];
    static bool init = false;

    if (!init) {
        /* initialize all to 0 */
        for (size_t i = 0; i <= 20; ++i)
            for (size_t j = 0; j <= 20; ++j)
                s[i][j] = 0;

        s[0][0] = 1;   /* base case */
        for (size_t i = 1; i <= 20; ++i) {
            for (size_t j = 1; j <= i; ++j) {
                s[i][j] = s[i-1][j-1] - (seq_t)(i-1) * s[i-1][j];
            }
        }
        init = true;
    }

    if (n < 1 || n > 20)
        return 0;  /* outside the defined range */

    return s[20][(size_t)n];
}
