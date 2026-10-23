/*
 * A169216  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=11, a(n)=11*10^(n-1) for 2≤n≤27;
 *            a(28)=9*∑_{i=1}^{27}a(i)−45*a(0)+1; for n≥29:
 *            a(n)=9*∑_{i=n-27}^{n-1}a(i)−45*a(n-28)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_capacity, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static idx_t size = 0;
    static idx_t cap = 0;
    static seq_t sum27 = 0;      /* sum of the last 27 values */

    if (size <= n) {
        ensure_capacity((size_t)(n + 1), &cap, (void **)&seq, sizeof(seq_t));

        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) {
                seq[0] = (seq_t)1;
            } else if (i <= 27) {
                seq[i] = seq_mul((seq_t)11, pow10(i - 1));
            } else if (i == 28) {
                /* compute sum of a[1..27] */
                sum27 = (seq_t)0;
                for (idx_t j = 1; j <= 27; ++j)
                    sum27 = sum27 + seq[j];
                seq[28] = seq_mul((seq_t)9, sum27);
                seq[28] = seq[28] - seq_mul((seq_t)45, seq[0]) + (seq_t)1;
                /* update sum27 for next step: sum of a[2..28] */
                sum27 = sum27 + seq[28] - seq[1];
            } else { /* i > 28 */
                seq[i] = seq_mul((seq_t)9, sum27);
                seq[i] = seq[i] - seq_mul((seq_t)45, seq[i - 28]);
                /* update sum27: now sum of a[i-26 .. i] */
                sum27 = sum27 + seq[i] - seq[i - 27];
            }
        }
        size = n + 1;
    }
    return seq[n];
}
