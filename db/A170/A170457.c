/*
 * A170457  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, * a(n) = 14 * sum_{k=1}^{min(44,n)} a(n-k) + p_n for 1
 *            <= n <= 44 * a(45) = 14 * sum_{k=1}^{44} a(45-k) + 105*a(0) + 1
 *            * a(n) = 14 * sum_{k=1}^{44} a(n-k) - 105*a(n-45) for n >= 46 *
 *            where p_n = 2 for 1 <= n <= 44, p_45 = 1, and p_0 = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    size_t N = (size_t)n;
    seq_t *a = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!a) return 0;  /* allocation failure, return 0 */

    a[0] = (seq_t)1;

    for (size_t i = 1; i <= N; ++i) {
        seq_t sum = (seq_t)0;

        if (i <= 44) {
            for (size_t k = 1; k <= i; ++k) {
                sum += a[i - k];
            }
            /* p_i = 2 for 1 <= i <= 44 */
            a[i] = (seq_t)14 * sum + (seq_t)2;
        } else if (i == 45) {
            for (size_t k = 1; k <= 44; ++k) {
                sum += a[i - k];
            }
            /* p_45 = 1; a[0] = 1 */
            a[i] = (seq_t)14 * sum + (seq_t)105 * a[0] + (seq_t)1;
        } else { /* i >= 46 */
            for (size_t k = 1; k <= 44; ++k) {
                sum += a[i - k];
            }
            a[i] = (seq_t)14 * sum - (seq_t)105 * a[i - 45];
        }
    }

    seq_t result = a[N];
    free(a);
    return result;
}
