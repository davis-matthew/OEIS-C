/*
 * A093967  a(n) = n * Pell(n).
 * https://oeis.org/A093967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * Pell(n) (Pell(0)=0, Pell(1)=1,
 *            Pell(n)=2*Pell(n−1)+Pell(n−2))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset 0 explicitly */
    if (n == 0) return 0;

    /* Pell numbers: P_0 = 0, P_1 = 1 */
    seq_t p_prev = 1;   /* P_{i-1} */
    seq_t p_prev2 = 0;  /* P_{i-2} */

    if (n == 1) return 1;  /* P_1 already known */

    /* Iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = p_prev + p_prev;    /* 2*P_{i-1} */
        temp += p_prev2;                 /* + P_{i-2} */

        p_prev2 = p_prev;   /* shift for next iteration */
        p_prev = temp;
    }

    /* Now p_prev == Pell(n) */
    seq_t result = (seq_t)n * p_prev;
    return result;
}
