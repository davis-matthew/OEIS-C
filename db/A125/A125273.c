/*
 * A125273  Eigensequence of triangle A085478: a(n) = Sum_{k=0..n-1}
 *          A085478(n-1,k)*a(k) for n > 0 with a(0) = 1.
 * https://oeis.org/A125273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = Σ_{k=0}^{n-1} binomial(n+k-1, n-k-1) * a(k)
 *            (n>0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* memoization buffers */
    static seq_t  *memo     = NULL;
    static bool   *computed = NULL;
    static size_t  cap      = 0;

    /* Ensure capacity for indices 0..n */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        while (n >= new_cap)
            new_cap *= 2;

        seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        bool   *new_comp = (bool *)realloc(computed, new_cap * sizeof(bool));

        for (size_t i = cap; i < new_cap; ++i) {
            new_mem[i]  = 0;
            new_comp[i] = false;
        }

        memo     = new_mem;
        computed = new_comp;
        cap      = new_cap;
    }

    /* Compute values up to n, in increasing order */
    for (idx_t i = 0; i <= n; ++i) {
        if (computed[i])
            continue;

        if (i == 0) {
            memo[0] = 1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t coeff = binom(i + k - 1, i - k - 1);
                sum += coeff * memo[k];
            }
            memo[i] = sum;
        }
        computed[i] = true;
    }

    return memo[n];
}
