/*
 * A227784  Least number of fourth powers which add to -1 mod n.
 * https://oeis.org/A227784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: BFS search for the least number of fourth powers whose sum
 * is congruent to -1 modulo n.  The residue set S = {x⁴ mod n | 0 ≤ x < n}
 * is constructed once per call, then a breadth‑first traversal from 0
 * to the target residue (n-1) gives the minimal number of terms. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 1) return 0;
    /* Any other n ≥ 2 */
    size_t N = (size_t)n;                /* local copy as size_t */

    /* ----- build S = unique fourth‑power residues mod n ----- */
    bool *mark = (bool *)calloc(N, sizeof(bool));
    if (!mark) return 0;                  /* allocation failure */

    idx_t *S = NULL;                      /* array of residues */
    size_t Scap = 0, Slen = 0;

    for (size_t x = 0; x < N; ++x) {
        uint64_t xx = (uint64_t)x * (uint64_t)x % n;
        uint64_t xxxx = xx * xx % n;
        size_t r = (size_t)xxxx;
        if (!mark[r]) {
            mark[r] = true;
            if (Slen == Scap) {
                Scap = Scap ? Scap * 2 : 4;
                idx_t *tmp = (idx_t *)realloc(S, Scap * sizeof(idx_t));
                if (!tmp) { free(mark); free(S); return 0; }
                S = tmp;
            }
            S[Slen++] = (idx_t)r;
        }
    }
    free(mark);

    /* ----- BFS for minimal number of terms ----- */
    int *dist = (int *)malloc(N * sizeof(int));
    if (!dist) { free(S); return 0; }
    for (size_t i = 0; i < N; ++i) dist[i] = -1;

    size_t *q = (size_t *)malloc(N * sizeof(size_t));
    if (!q) { free(S); free(dist); return 0; }

    size_t front = 0, back = 0;
    q[back++] = 0;
    dist[0] = 0;
    size_t target = N - 1;                  /* the residue -1 mod n */

    while (front < back && dist[target] == -1) {
        size_t u = q[front++];
        int du = dist[u];
        for (size_t i = 0; i < Slen; ++i) {
            size_t v = u + S[i];
            if (v >= N) v -= N;
            if (dist[v] == -1) {
                dist[v] = du + 1;
                q[back++] = v;
            }
        }
    }

    seq_t ans = (dist[target] >= 0) ? (seq_t)dist[target] : (seq_t)0;
    free(q);
    free(dist);
    free(S);
    return ans;
}
