/*
 * A053251  Coefficients of the '3rd-order' mock theta function psi(q).
 * https://oeis.org/A053251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 135, u32 n <= 430, u64 n <= 1483,
 *            u128 n <= 5418, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A053251: coefficients of the 3rd‑order mock theta function ψ(q).
 *
 * Formula: a(n) = ∑_{m=1}^{⌊√n⌋} p_o(n–m², 2m–1)
 * where p_o(N, L) is the number of partitions of N into odd parts not exceeding L.
 */
#include <stdlib.h>
#include <math.h>

static seq_t *dp = NULL;
static seq_t *a_arr = NULL;
static const idx_t LIMIT = 10000U;   /* maximum n requested in the kernel */

static void precompute(idx_t limit)
{
    if (dp != NULL) return;          /* already done */

    dp    = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    a_arr = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (dp == NULL || a_arr == NULL) return;   /* allocation failed */

    dp[0] = 1;                       /* empty partition */

    for (idx_t m = 1; m * m <= limit; ++m) {
        const idx_t odd = 2 * m - 1;

        /* add unlimited copies of the odd part 'odd' */
        for (idx_t k = odd; k <= limit; ++k)
            dp[k] += dp[k - odd];

        /* add contributions of this m to all n ≥ m² */
        const idx_t start = m * m;
        for (idx_t n = start; n <= limit; ++n) {
            const idx_t rem = n - start;
            a_arr[n] += dp[rem];
        }
    }
}

seq_t a(idx_t n)
{
    if (n > LIMIT) /* sequence is defined only for n ≤ 10000 in this challenge */
        return 0;
    precompute(LIMIT);
    return a_arr[n];
}
