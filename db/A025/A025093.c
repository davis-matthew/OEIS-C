/*
 * A025093  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (Lucas numbers), t = (odd natural numbers).
 * https://oeis.org/A025093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025093 – Lucas‑weighted sum of odd numbers
 *
 * a(n) = Σ_{i=1}^{⌈n/2⌉} L_i · (2·(n-i+1) + 1)
 * where L_i is the i‑th Lucas number (L_1=1, L_2=3, L_i=L_{i-1}+L_{i-2})
 * and the odd numbers are 3,5,7, …  (t(m)=2m+1).
 *
 * Offset: 1  (a(1) = 3)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* ceil(n/2) */
    idx_t k = (n + 1) >> 1;
    seq_t sum = 0;

    seq_t L_prev2 = (seq_t)1;   /* L_1 */
    seq_t L_prev1 = (seq_t)3;   /* L_2 */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t L;
        if (i == 1) {
            L = L_prev2;
        } else if (i == 2) {
            L = L_prev1;
        } else {
            L = L_prev1 + L_prev2;
            L_prev2 = L_prev1;
            L_prev1 = L;
        }

        seq_t pos = (seq_t)(n - i + 1);
        seq_t odd = (seq_t)(2 * pos + 1);   /* t(n-i+1) = 2*(n-i+1)+1 */
        sum += L * odd;
    }

    return sum;
}
