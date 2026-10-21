/*
 * A072358  Number of cubefree numbers <= n which are not squarefree.
 * https://oeis.org/A072358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Count{ numbers ≤ n that are cubefree but NOT squarefree }
 * Range:     u8 n <= 1131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_icbrt, mobius_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = A060431(n) - A013928(n+1)                                              */
/*           (cubefree count – squarefree count)                                  */
/*   Equivalent: a(n) = Σ_{k=1..n} (μ(k³) * I[k³ ≤ n]) - Σ_{k=1..n} (μ(k²) * I[k² ≤ n]) */

#include <stddef.h>   /* for size_t (not used directly but required by headers) */
#include <stdbool.h>  /* included for consistency with the build utils */
#include "../utils/floor_icbrt.h"
#include "../utils/mobius_small.h"

/* Integer floor square root, works for values up to 2⁶⁴-1. */
static idx_t floor_isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Count squarefree numbers ≤ n: Σ μ(d) * ⌊n / d²⌋ */
    idx_t sqrt_n = floor_isqrt(n);
    seq_t sqfree = 0;
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        int mu = mobius_small(d);
        if (mu == 0) continue;
        seq_t term = (seq_t)(n / (d * d));
        if (mu == 1) sqfree += term;
        else         sqfree -= term; /* mu == -1 */
    }

    /* Count cubefree numbers ≤ n: Σ μ(d) * ⌊n / d³⌋ */
    idx_t cbrt_n = floor_icbrt(n);
    seq_t cubefree = 0;
    for (idx_t d = 1; d <= cbrt_n; ++d) {
        int mu = mobius_small(d);
        if (mu == 0) continue;
        seq_t term = (seq_t)(n / (d * d * d));
        if (mu == 1) cubefree += term;
        else         cubefree -= term;
    }

    /* Numbers that are cubefree but not squarefree. */
    return cubefree - sqfree;
}
