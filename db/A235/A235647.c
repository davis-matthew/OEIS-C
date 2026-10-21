/*
 * A235647  a(n) is the number of cycles of the permutation relating the
 *          lexicographic order to the colexicographic order, for the set of
 *          pairs (i,j), 1 <= i <= j <= n.
 * https://oeis.org/A235647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 334, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235647 – compute the number of cycles of the permutation mapping the
   lexicographic order of the upper‑triangular pairs (i,j), 1≤i≤j≤n,
   to the colexicographic order. */
/* The algorithm builds the permutation P(k)=C(i,j) for each row‑major
   index k, then counts its cycles by a simple DFS over the permutation. */
/* Complexity: O(n²) time and O(n²) memory (≈150 MB for n=10,000). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static inline int visited_get(idx_t pos, const unsigned char *vis)
{
    /* pos: 0‑based position (0 … M-1) */
    return (vis[pos >> 3] >> (pos & 7)) & 1;
}
static inline void visited_set(idx_t pos, unsigned char *vis)
{
    vis[pos >> 3] |= (1u << (pos & 7));
}

seq_t a(idx_t n)
{
    /* total number of pairs */
    idx_t M = (idx_t)n * ((idx_t)n + 1ULL) / 2ULL;

    /* visited bit‑set */
    size_t vbytes = (M + 7) / 8;
    unsigned char *visited = (unsigned char*)calloc(vbytes, 1);

    /* array of row numbers for each row‑major index (1‑based) */
    uint16_t *row = (uint16_t*)malloc((size_t)(M + 1) * sizeof(uint16_t));

    /* start[i] = the first index of row i in row‑major ordering */
    idx_t *start = (idx_t*)malloc((size_t)(n + 2) * sizeof(idx_t));
    start[1] = 1;
    for (idx_t i = 2; i <= n; ++i)
        start[i] = start[i - 1] + (n - (i - 1) + 1); /* n-i+2 */
    start[n + 1] = M + 1;

    /* fill row array */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t from = start[i];
        idx_t to   = start[i + 1];
        for (idx_t k = from; k < to; ++k)
            row[k] = (uint16_t)i;
    }

    /* count cycles */
    idx_t cycles = 0;
    for (idx_t k = 1; k <= M; ++k) {
        if (!visited_get(k - 1, visited)) {
            idx_t cur = k;
            do {
                visited_set(cur - 1, visited);
                uint16_t i_val = row[cur];
                idx_t j = (idx_t)i_val + cur - start[i_val];
                seq_t dest = ((seq_t)(j - 1) * j) / 2 + i_val;
                cur = (idx_t)dest;
            } while (!visited_get(cur - 1, visited));
            ++cycles;
        }
    }

    /* free temporary arrays (optional, may be omitted for speed) */
    free(visited);
    free(row);
    free(start);

    return (seq_t)cycles;
}
