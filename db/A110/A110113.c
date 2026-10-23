/*
 * A110113  Diagonal sums of A083856.
 * https://oeis.org/A110113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^n * ((1+√(4*(n-k)+1))/2)^k / √(4*(n-k)+1) * -
 *            ((1-√(4*(n-k)+1))/2)^k / √(4*(n-k)+1) * * This implementation
 *            uses the equivalent integer form * a(n) = Σ_{k=0}^n D_k(n-k) *
 *            where * D_k(m) = Σ_{j=0}^{⌊(k-1)/2⌋} C(k-j-1,j) * m^j * and C()
 *            is the binomial coefficient
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {          /* D_0(m) = 0 */
            continue;
        }

        idx_t m = n - k;
        seq_t sm = (seq_t)m;   /* cast m to seq_t once */

        seq_t val = 0;
        int maxj = (int)((k - 1) / 2);

        seq_t pow   = 1;   /* m^0 */
        seq_t binom = 1;   /* C(k-j-1, j) for j = 0 */

        for (int j = 0; j <= maxj; ++j) {
            val += binom * pow;

            if (j < maxj) {
                int next_j = j + 1;

                /* update the binomial coefficient:
                   C(k - next_j - 1, next_j) = C(k - j - 1, j) *
                   ((k - 2*next_j + 1)*(k - 2*next_j)) /
                   ((k - next_j)*next_j)               */
                seq_t num1 = (seq_t)(k - 2 * next_j + 1);
                seq_t num2 = (seq_t)(k - 2 * next_j);
                seq_t den1 = (seq_t)(k - next_j);
                seq_t den2 = (seq_t)next_j;

                binom = (binom * num1 * num2) / (den1 * den2);

                /* increment the power of m */
                pow = pow * sm;
            }
        }

        result += val;
    }

    return result;
}
