/*
 * A168930  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=11*Σ_{i=1}^{min(n,21)} a(n-i)+2 (1 ≤ n ≤ 21)
 *            a(22)=11*Σ_{i=1}^{21} a(22-i)-66*a(0)+1 a(n)=11*Σ_{i=1}^{21}
 *            a(n-i)-66*a(n-22) (n ≥ 23)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *a = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!a) return (seq_t)0;          /* out of memory */

    a[0] = 1;

    /* compute terms 1..21 */
    for (size_t m = 1; m <= (N < 22 ? N : 21); ++m) {
        seq_t s = 0;
        size_t maxk = (m < 21 ? m : 21);
        for (size_t k = 1; k <= maxk; ++k) s += a[m - k];
        a[m] = 11 * s + 2;
    }

    if (N < 22) {
        seq_t res = a[N];
        free(a);
        return res;
    }

    /* a(22) */
    seq_t sum_win = 0;
    for (size_t i = 1; i <= 21; ++i) sum_win += a[i];
    a[22] = 11 * sum_win - 66 * a[0] + 1;

    if (N == 22) {
        seq_t res = a[22];
        free(a);
        return res;
    }

    /* update window to a[2]..a[22] */
    sum_win = sum_win + a[22] - a[1];

    for (size_t m = 23; m <= N; ++m) {
        a[m] = 11 * sum_win - 66 * a[m - 22];
        sum_win = sum_win + a[m] - a[m - 21];
    }

    seq_t res = a[N];
    free(a);
    return res;
}
