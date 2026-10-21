/*
 * A097632  a(n) = 2^n * Lucas(n) * (n-1)!.
 * https://oeis.org/A097632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * Lucas(n) * (n-1)! (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* not defined for offset 1, but return 0 for safety */
        return (seq_t)0;

    /* Lucas numbers: L0 = 2, L1 = 1, Ln = Ln-1 + Ln-2 */
    seq_t lucas_prev2 = (seq_t)2;   /* L0 */
    seq_t lucas_prev1 = (seq_t)1;   /* L1 */
    seq_t lucas_n = lucas_prev1;    /* default for n=1 */

    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i) {
            lucas_n = lucas_prev2 + lucas_prev1;
            lucas_prev2 = lucas_prev1;
            lucas_prev1 = lucas_n;
        }
    }

    /* (n-1)!  */
    seq_t fact = (seq_t)1;
    if (n > 1) {
        for (idx_t i = 2; i <= n - 1; ++i) {
            fact *= (seq_t)i;
        }
    }

    /* 2^n  */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    /* Result */
    return pow2 * lucas_n * fact;
}
