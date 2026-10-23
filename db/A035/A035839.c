/*
 * A035839  Coordination sequence for A_13 lattice.
 * https://oeis.org/A035839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 55, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A035839: Coordination sequence for A_13 lattice.
 * Formula: a(0) = 1,
 *          a(n) = Σ_{d=1..13} C(14,d) * C(n-1, d-1) * C(n+13-d, n)
 *          (for n ≥ 1, n is an even norm/2)
 *
 * Implementation uses multiplicative binomial computation to keep all
 * intermediate values small enough for the 64‑bit build while remaining
 * exact on arbitrary‑precision builds.
 */
#include <stddef.h>
#include <stdint.h>

static const seq_t comb14[15] = {
    /* C(14,0) */ 1,
    /* C(14,1) */ 14,
    /* C(14,2) */ 91,
    /* C(14,3) */ 364,
    /* C(14,4) */ 1001,
    /* C(14,5) */ 2002,
    /* C(14,6) */ 3003,
    /* C(14,7) */ 3432,
    /* C(14,8) */ 3003,
    /* C(14,9) */ 2002,
    /* C(14,10) */ 1001,
    /* C(14,11) */ 364,
    /* C(14,12) */ 91,
    /* C(14,13) */ 14,
    /* C(14,14) */ 1
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* maximum d is 13, but cannot exceed n */
    idx_t maxd = 13;
    if (n < 13) maxd = (idx_t)n;

    seq_t result = (seq_t)0;

    for (idx_t d = 1; d <= maxd; ++d)
    {
        /* C(n-1, d-1) */
        seq_t c1 = (seq_t)1;
        for (idx_t k = 1; k <= d-1; ++k)
        {
            c1 = c1 * (seq_t)(n - k) / (seq_t)k;
        }

        /* C(n+13-d, 13-d) which equals C(n+13-d, n) */
        seq_t c2 = (seq_t)1;
        idx_t choose = 13 - d;  /* choose smaller side */
        for (idx_t k = 1; k <= choose; ++k)
        {
            c2 = c2 * (seq_t)(n + k) / (seq_t)k;
        }

        /* multiply the three components */
        seq_t term = comb14[d] * c1;
        term = term * c2;

        result += term;
    }

    return result;
}
