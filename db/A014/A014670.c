/*
 * A014670  G.f.:
 *          (1+x)*(1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)/((1-x^2)*(1-x^4)*(1-x^6)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A014670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{S ⊆ {1,3,5,7,9}} P(n - ΣS), where P(m) is the number
 *            of partitions of m into unlimited parts {2,4,6,8,10}. The
 *            generating function is (1+x)(1+x³)(1+x⁵)(1+x⁷)(1+x⁹) /
 *            (1−x²)(1−x⁴)(1−x⁶)(1−x⁸)(1−x¹⁰)
 * Range:     u8 n <= 25, u16 n <= 114, u32 n <= 1872, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute P(m) for 0 ≤ m ≤ n via elementary DP. */
    seq_t *P = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!P) return (seq_t)0;          /* Allocation failure: unlikely. */

    for (idx_t i = 0; i <= n; ++i)
        P[i] = (seq_t)0;
    P[0] = (seq_t)1;                      /* One partition of 0. */

    const idx_t coins[5] = {2, 4, 6, 8, 10};
    for (int c = 0; c < 5; ++c) {
        idx_t coin = coins[c];
        for (idx_t j = coin; j <= n; ++j)
            P[j] += P[j - coin];
    }

    /* Sum over all subsets of the odd numbers 1,3,5,7,9. */
    const idx_t odds[5] = {1, 3, 5, 7, 9};
    seq_t total = (seq_t)0;
    for (idx_t mask = 0; mask < 32; ++mask) {
        idx_t sum = 0;
        for (int k = 0; k < 5; ++k)
            if (mask & (1ULL << k))
                sum += odds[k];
        if (sum <= n)
            total += P[n - sum];
    }

    free(P);
    return total;
}
