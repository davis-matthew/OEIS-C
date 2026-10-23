/*
 * A011521  Stirling numbers of first kind S1(11,n).
 * https://oeis.org/A011521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = Stirling1st(11,n) (sign‑sensitive)
 * Range:     i8 none, i16 none, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*   s(n,k) = s(n-1,k-1) - (n-1)*s(n-1,k),  s(1,1)=1  */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t s[12];  /* indices 0..11, index 0 unused */

    if (!initialized) {
        for (idx_t i = 0; i < 12; ++i) s[i] = 0;

        s[1] = 1;                     /* base: s(1,1) = 1 */

        for (idx_t i = 2; i <= 11; ++i) {
            for (idx_t k = i; k > 0; --k) {
                seq_t term = (seq_t)(i - 1) * s[k];
                s[k] = s[k - 1] - term;
            }
        }
        initialized = true;
    }

    if (n > 11 || n == 0) return (seq_t)0;
    return s[n];
}
