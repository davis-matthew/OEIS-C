/*
 * A011567  Stirling numbers of second kind S2(18,n).
 * https://oeis.org/A011567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = S2(18,n) (Stirling numbers of the second kind)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n)
{
    static seq_t S[19][19] = {0};
    static bool init = false;

    if (!init) {
        S[0][0] = 1;
        for (idx_t i = 1; i <= 18; ++i) {
            S[i][0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                S[i][k] = S[i-1][k-1] + (seq_t)k * S[i-1][k];
            }
        }
        init = true;
    }

    if (n <= 18) {
        return S[18][n];
    }
    return 0;   /* S2(18,n) = 0 for n > 18 */
}
