/*
 * A022330  Index of 3^n within sequence of numbers of form 2^i*3^j (A003586).
 * https://oeis.org/A022330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i,j) with i>=0, j>=0 and 2^i·3^j <= 3^n.
 *            It can be computed as a(n) = Σ_{i=0}^{⌊n/α⌋} (⌊n - i·α⌋ + 1),
 *            where α = ln 2 / ln 3 ≈ 0.6309297535714574
 * Range:     u8 n <= 17, u16 n <= 286, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* constant α = ln 2 / ln 3, pre‑computed to high precision */
    const double alpha = 0.6309297535714574;

    /* i will run up to floor(n/α) */
    idx_t i_max = (idx_t)( (double)n / alpha + 1e-12 ); /* add tiny epsilon to avoid off‑by‑one */

    seq_t count = 0;
    for (idx_t i = 0; i <= i_max; ++i)
    {
        /* compute floor(n - i·α) */
        double t = (double)n - (double)i * alpha;
        idx_t j_max = (idx_t)(t + 1e-12);   /* floor(t) – tiny epsilon */

        /* safety guard – j_max is at most n, but should never exceed it */
        if (j_max > n) j_max = n;

        count += (seq_t)j_max + 1;          /* add numbers for this i */
    }
    return count;
}
