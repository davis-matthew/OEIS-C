/*
 * A211263  Number of integer pairs (x,y) such that 0<x<y<=n and
 *          x*y=floor(n/2).
 * https://oeis.org/A211263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ τ(⌊n/2⌋) / 2 ⌋ where τ(m) is the number of positive
 *            divisors of m. Count the pairs (x,y) with 0 < x < y ≤ n and x*y
 *            = ⌊n/2⌋
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *divcnt = NULL;      /* divisor counts up to current maximum m */
static idx_t  max_precalc = 0;    /* largest m for which divcnt is computed */

/* Recompute divisor counts for all m ≤ limit.  Complexity O(limit log limit). */
static void recompute_divcounts(idx_t limit)
{
    free(divcnt);
    divcnt = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!divcnt) return;           /* in practice we ignore allocation failure */

    for (idx_t i = 0; i <= limit; ++i) divcnt[i] = 0;

    for (idx_t i = 1; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i)
            divcnt[j] += (seq_t)1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1, below it be zero */

    idx_t m = n / 2;                    /* floor(n/2) */
    if (m == 0) return 0;               /* n = 1 or 2 → m = 0 → result 0 */

    if (m > max_precalc) {
        max_precalc = m;
        recompute_divcounts(max_precalc);
    }

    /* number of unordered factor pairs = τ(m)/2 (rounded down) */
    return divcnt[m] >> 1;
}
