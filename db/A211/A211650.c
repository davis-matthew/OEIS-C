/*
 * A211650  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^3 < x^3 + y^3.
 * https://oeis.org/A211650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 - Σ_{w=1..n} T(w) where T(w) is the number of ordered
 *            pairs (x,y) with 1 ≤ x,y ≤ n and x^3 + y^3 ≤ w^3
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* convert to size_t for array sizing */
    size_t N = (size_t)n;

    /* cube[i] = i^3 */
    seq_t *cube = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!cube) return 0; /* allocation failed */

    for (size_t i = 1; i <= N; ++i)
        cube[i] = (seq_t)i * (seq_t)i * (seq_t)i;

    /* Y[x] holds the current maximum y such that
       cube[x] + cube[y] <= current threshold. */
    idx_t *Y = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!Y) { free(cube); return 0; }

    for (size_t i = 1; i <= N; ++i) Y[i] = 0;

    seq_t total_pairs = 0; /* current f(w) value */
    seq_t sum_f       = 0; /* cumulative Σ f(w) */

    for (size_t w = 1; w <= N; ++w)
    {
        seq_t bound = cube[w];

        /* increase Y[x] as far as allowed by the new bound */
        for (size_t x = 1; x <= N; ++x)
        {
            while (Y[x] < (idx_t)N &&
                   cube[x] + cube[(size_t)(Y[x] + 1)] <= bound)
            {
                ++Y[x];
                ++total_pairs;
            }
        }

        sum_f += total_pairs;
    }

    /* compute n^3 */
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;

    seq_t result = n3 - sum_f;

    free(cube);
    free(Y);
    return result;
}
