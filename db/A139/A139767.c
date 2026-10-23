/*
 * A139767  Let e = 2n+1; consider the cyclotomic cosets C_i of 2 mod e; a(n)
 *          = maximal value of minimal number of copies of C_1 needed to add
 *          together to get any coset C_i.
 * https://oeis.org/A139767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{r∈ℤ/(2n+1)ℤ} min_k { r ≡ c₁+⋯+c_k (mod 2n+1) } where
 *            C₁ = {2^j mod (2n+1) | j≥0}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* modulus e = 2n+1 (always odd) */
    idx_t e = 2 * n + 1;

    /* ---- build C₁ (cyclotomic coset of 1) --------------------------- */
    idx_t *S = (idx_t *)malloc(e * sizeof(idx_t));
    if (!S) return (seq_t)0;
    idx_t val = 1 % e;
    size_t ssz = 0;
    do {
        S[ssz++] = val;
        val = (idx_t)(((unsigned __int128)val * 2) % e);
    } while (val != 1 && ssz < e);

    /* ---- BFS to compute minimal number of C₁ elements for each residue */
    int *dist = (int *)malloc(e * sizeof(int));
    if (!dist) { free(S); return (seq_t)0; }
    for (size_t i = 0; i < e; ++i) dist[i] = -1;

    idx_t *q = (idx_t *)malloc(e * sizeof(idx_t));
    if (!q) { free(S); free(dist); return (seq_t)0; }
    size_t head = 0, tail = 0;

    dist[0] = 0;
    q[tail++] = 0;

    while (head < tail) {
        idx_t x = q[head++];
        int d = dist[x] + 1;
        for (size_t i = 0; i < ssz; ++i) {
            idx_t y = x + S[i];
            if (y >= e) y -= e;
            if (dist[y] == -1) {
                dist[y] = d;
                q[tail++] = y;
            }
        }
    }

    /* ---- find maximum distance among all residues ------------------- */
    int maxdist = 0;
    for (size_t i = 1; i < e; ++i) {
        if (dist[i] > maxdist) maxdist = dist[i];
    }

    free(S);
    free(dist);
    free(q);

    return (seq_t)maxdist;
}
