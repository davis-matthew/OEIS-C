/*
 * A011571  Stirling numbers of second kind S2(22,n).
 * https://oeis.org/A011571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling second‑kind number S(22,n) (* n is 1‑based *)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Store S(22,k) for 1 ≤ k ≤ 22.  An unsigned zero index is unused. */
    static seq_t S[23] = {0};
    static bool init = false;

    if (!init) {
        /* Compute rows 1…22 using S(i,k)=k*S(i-1,k)+S(i-1,k-1)   */
        S[1] = 1;                /* S(1,1) = 1 */
        for (idx_t i = 2; i <= 22; ++i) {
            for (idx_t k = i; k >= 1; --k) {
                S[k] = S[k] * (seq_t)k + S[k-1];
            }
        }
        init = true;
    }

    if (n < 1 || n > 22)
        return (seq_t)0;
    return S[n];
}
