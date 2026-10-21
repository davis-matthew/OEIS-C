/*
 * A207507  Diameters of quadratic unitary Cayley graphs on the integers
 *          modulo n.
 * https://oeis.org/A207507
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207507
 *  a(n) = diameter of the quadratic–unitary Cayley graph of the additive
 *  group Z/nZ.  The graph has vertex set Z/nZ and an edge from x to
 *  x±u for every unit u of Z/nZ that is also a quadratic residue.
 *  The value a(n) is the maximum, over all non‑zero residues, of the
 *  minimum number of such generators needed to express that residue.
 *
 *  Algorithm:
 *  1. Enumerate all units of Z/nZ and keep those that are quadratic
 *     residues; store them in an array `steps` (each step is used in
 *     both ± directions).
 *  2. Perform a breadth‑first search from 0 using the steps as generators.
 *     The distance array `dist` stores the length of the shortest path
 *     from 0 to each residue.
 *  3. The diameter is the maximum value in `dist` for already visited
 *     vertices.
 *
 *  The routine uses only integer arithmetic; memory is allocated
 *  per call and released before return.  It works for all seq_t /
 *  idx_t types supported by the build system.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* the sequence is defined only for n ≥ 2 */

    /* ----------- 1. Build the set of quadratic–unit generators ----------- */
    /* Allocate a temporary buffer sufficient for all units. */
    idx_t *steps = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    if (!steps) return 0;  /* allocation failure */

    size_t step_cnt = 0;
    for (idx_t a_val = 1; a_val < n; ++a_val) {
        if (gcd((seq_t)a_val, (seq_t)n) != (seq_t)1)
            continue;               /* not a unit */

        seq_t sq = ((seq_t)a_val * a_val) % (seq_t)n;
        idx_t sqi = (idx_t)sq;      /* square is < n */

        /* keep each quadratic residue only once */
        bool exists = false;
        for (size_t i = 0; i < step_cnt; ++i) {
            if (steps[i] == sqi) { exists = true; break; }
        }
        if (!exists) steps[step_cnt++] = sqi;
    }

    /* ----------- 2. Breadth‑first search over Z/nZ ----------------------- */
    idx_t *dist = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    unsigned char *vis = (unsigned char*)calloc((size_t)n, sizeof(unsigned char));
    idx_t *queue = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    if (!dist || !vis || !queue) {
        free(steps);
        free(dist); free(vis); free(queue);
        return 0;
    }

    size_t qhead = 0, qtail = 0;
    dist[0] = 0;
    vis[0] = 1;
    queue[qtail++] = 0;

    while (qhead < qtail) {
        idx_t cur   = queue[qhead++];
        idx_t curd  = dist[cur];

        for (size_t si = 0; si < step_cnt; ++si) {
            idx_t step = steps[si];

            idx_t nxt = (cur + step) % n;
            if (!vis[nxt]) {
                vis[nxt] = 1;
                dist[nxt] = curd + 1;
                queue[qtail++] = nxt;
            }

            idx_t nxt2 = (cur + (n - step)) % n;
            if (!vis[nxt2]) {
                vis[nxt2] = 1;
                dist[nxt2] = curd + 1;
                queue[qtail++] = nxt2;
            }
        }
    }

    /* ----------- 3. Determine the diameter -------------------------------- */
    idx_t maxd = 0;
    for (idx_t i = 1; i < n; ++i)
        if (dist[i] > maxd) maxd = dist[i];

    /* ----------- 4. Clean‑up and return ------------------------------------ */
    free(steps);
    free(dist);
    free(vis);
    free(queue);
    return (seq_t)maxd;
}
