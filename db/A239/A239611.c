/*
 * A239611  a(n) = Sum_{0 < x,y <= n and gcd(x^2 + y^2, n)=1} gcd(x^2 + y^2 -
 *          1, n).
 * https://oeis.org/A239611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{0 < x,y <= n, gcd(x^2 + y^2, n) = 1} gcd(x^2 + y^2 -
 *            1, n)
 * Range:     u8 n <= 11, u16 n <= 95, u32 n <= 2101, u64 n <= 2101,
 *            u128 n <= 2101, big any n
 * Helpers:   _gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_gcd64.h"

/* Euclidean gcd for 64‑bit unsigned numbers */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety; n≥1 in practice */

    uint64_t N = (uint64_t)n;              /* use 64‑bit for internal work */
    size_t NS = (size_t)n;                /* array size */

    /* compute x^2 mod N for 1 ≤ x ≤ n */
    uint64_t *sq = (uint64_t*)malloc((NS + 1) * sizeof(uint64_t));
    if (!sq) return (seq_t)0;              /* allocation failure */
    for (size_t i = 1; i <= NS; ++i) {
        uint64_t v = (uint64_t)i * (uint64_t)i;
        sq[i] = v % N;
    }

    /* precompute gcd(r,N) for all residues 0..N-1 */
    uint64_t *g = (uint64_t*)malloc(N * sizeof(uint64_t));
    uint64_t *d = (uint64_t*)malloc(N * sizeof(uint64_t));
    if (!g || !d) {
        free(sq); free(g); free(d);
        return (seq_t)0;
    }
    for (uint64_t r = 0; r < N; ++r) {
        g[r] = _gcd64(r, N);
        uint64_t r1 = (r + N - 1) % N;     /* (r-1) mod N */
        d[r] = _gcd64(r1, N);              /* gcd((r-1) mod N, N) */
    }

    /* main double loop */
    uint64_t sum = 0;
    for (size_t xi = 1; xi <= NS; ++xi) {
        uint64_t ax = sq[xi];
        for (size_t yi = 1; yi <= NS; ++yi) {
            uint64_t r = ax + sq[yi];
            if (r >= N) r -= N;            /* (ax + sq[yi]) mod N */
            if (g[r] == 1) {
                sum += d[r];
            }
        }
    }

    /* clean up */
    free(sq); free(g); free(d);

    return (seq_t)sum;
}
