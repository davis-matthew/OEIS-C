/*
 * A120884  (1/8)*number of lattice points with odd indices in a cubic lattice
 *          inside a sphere around the origin with radius 2*n.
 * https://oeis.org/A120884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of ordered triples of positive odd integers *
 *            (x, y, z) with x² + y² + z² < (2·n)². * Equivalently, a(n) = #
 *            {(u, v, w) ≥ 0 : * u(u+1) + v(v+1) + w(w+1) ≤ ⌊((2n)² - 4)/4⌋ }.
 *            * The implementation counts these triples by iterating over the
 *            * first two coordinates and using a moving...
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   count_triples
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_triples.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute M = floor(((2n)² - 4)/4) as defined in the formula. */
    idx_t R = 2 * n;
    seq_t R2 = (seq_t)R * (seq_t)R;
    seq_t M = (R2 <= (seq_t)4) ? 0 : (R2 - (seq_t)4) / (seq_t)4;

    /* Find Kmax: largest k with k(k+1) ≤ M. */
    idx_t k = 0;
    seq_t tk = 0;
    while (tk <= M) {
        ++k;
        tk = (seq_t)k * (seq_t)k + (seq_t)k;
    }
    idx_t Kmax = k - 1;

    /* Allocate array of t[k] = k(k+1). */
    seq_t *t = (seq_t *)malloc((Kmax + 1) * sizeof(seq_t));
    if (!t) return 0;  /* unlikely, but safe */
    t[0] = 0;
    for (idx_t i = 1; i <= Kmax; ++i)
        t[i] = (seq_t)i * (seq_t)i + (seq_t)i;

    /* Count triples and free temporary array. */
    seq_t result = count_triples(Kmax, t, M);
    free(t);
    return result;
}
