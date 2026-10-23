/*
 * A168949  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * For n ≥ 1 * a(n) = ∑_{k=1}^{min(21,n)} 30·a(n−k) *
 *            − (n ≥ 22 ? 465·a(n−22) : 0) * + (n ≤ 21 ? 2 : (n == 22 ? 1 :
 *            0)) * * The sequence satisfies the recurrence * a(n) =
 *            30·a(n−1)+⋯+30·a(n−21)−465·a(n−22) for n > 22, * with the first
 *            23 terms determined by the initial values
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;

    /* Grow the array to at least n+1 elements. */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0) {
                seq[0] = 1;
                continue;
            }

            seq_t sum = 0;
            size_t start = (i > 21) ? i - 21 : 0;
            for (size_t j = start; j < i; ++j)
                sum += 30 * seq[j];          /* 30 · a(j) */

            if (i >= 22)
                sum -= 465 * seq[i - 22];    /* −465 · a(i−22) */

            seq_t term = 0;
            if (i <= 21)
                term = 2;
            else if (i == 22)
                term = 1;

            seq[i] = sum + term;
        }
        cap = new_cap;
    }

    return seq[n];
}
