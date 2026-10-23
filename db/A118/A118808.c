/*
 * A118808  Number of partitions of n having exactly one part with
 *          multiplicity 3.
 * https://oeis.org/A118808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n having exactly one part with
 *            multiplicity 3. This is computed by dynamic programming over
 *            part sizes
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (!n) return 0;           /* a(0) = 0 */
    size_t N = (size_t)n;       /* cast to size_t for array indices */

    /* allocate four arrays of size N+1 */
    seq_t *f     = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *g     = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *new_f = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *new_g = (seq_t*)malloc((N + 1) * sizeof(seq_t));

    if (!f || !g || !new_f || !new_g) {
        free(f); free(g); free(new_f); free(new_g);
        return 0;
    }

    /* initialise arrays */
    for (size_t i = 0; i <= N; ++i) {
        f[i]     = 0;
        g[i]     = 0;
        new_f[i] = 0;
        new_g[i] = 0;
    }
    f[0] = 1;   /* empty partition */

    for (size_t j = 1; j <= N; ++j) {
        /* ---- compute new_f (state 0, no triple yet) ---- */
        /* start with copies of f (allow k = 0) */
        for (size_t i = 0; i <= N; ++i) new_f[i] = f[i];

        /* allow unlimited copies of part j */
        for (size_t s = j; s <= N; ++s) new_f[s] += new_f[s - j];

        /* remove contributions where exactly 3 copies of j are used */
        if (3 * j <= N) {
            for (size_t s = 3 * j; s <= N; ++s) new_f[s] -= f[s - 3 * j];
        }

        /* ---- compute new_g (state 1, already used a single triple) ---- */
        /* start with copies of g (allow k = 0) */
        for (size_t i = 0; i <= N; ++i) new_g[i] = g[i];

        /* allow unlimited copies of part j for g (exclude k = 3) */
        for (size_t s = j; s <= N; ++s) new_g[s] += new_g[s - j];

        /* subtract k = 3 from the g state */
        if (3 * j <= N) {
            for (size_t s = 3 * j; s <= N; ++s) new_g[s] -= g[s - 3 * j];

            /* add contributions where part j provides the single triple */
            for (size_t s = 3 * j; s <= N; ++s) new_g[s] += f[s - 3 * j];
        }

        /* swap arrays for next iteration */
        seq_t *tmp;
        tmp = f;     f = new_f;     new_f = tmp;
        tmp = g;     g = new_g;     new_g = tmp;
    }

    seq_t result = g[N];

    free(f); free(g); free(new_f); free(new_g);
    return result;
}
