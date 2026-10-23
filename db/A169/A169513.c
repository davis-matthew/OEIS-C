/*
 * A169513  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function:
 *   a(0) = 1
 *   For 1 ≤ n ≤ 33 :  a(n) = 2 + 18 * Σ_{i=1}^{n}   a(n-i)
 *   For n ≥ 34       : a(n) = 18 * Σ_{i=1}^{33} a(n-i) – 171 * a(n-34)
 * (The 1 in the first term of the sum appears only for n = 0 or n = 34, which is
 *  handled by the formula above.)                                          */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Buffer of the last 35 values; indices are taken mod 35.         */
    seq_t buf[35] = {0};
    buf[0] = 1;                /* a(0) = 1 */
    if (n == 0) return buf[0];

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum33 = 0;
        for (idx_t j = 1; j <= 33 && i >= j; ++j)
            sum33 += buf[(i - j) % 35];

        seq_t val;
        if (i == 34) /* P_i = 1 for i = 34 */
            val = 1 + 18 * sum33 - 171 * buf[(i - 34) % 35];
        else if (i <= 33)          /* P_i = 2 for 1 ≤ i ≤ 33 */
            val = 2 + 18 * sum33;
        else                       /* i > 34, P_i = 0 */
            val = 18 * sum33 - 171 * buf[(i - 34) % 35];

        buf[i % 35] = val;       /* store current value */
    }

    return buf[n % 35];
}
