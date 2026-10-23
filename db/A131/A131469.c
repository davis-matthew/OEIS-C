/*
 * A131469  Grundy numbers of one pile short global nim.
 * https://oeis.org/A131469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{ G[n‑k][k] | 1 <= k <= n }, where G[n][m] = mex{
 *            G[n‑k][k] | 1 <= k <= n, k != m }, with G[0][*] = 0
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t max_n = 0;
    static seq_t *a_arr = NULL;
    static uint16_t **G = NULL;

    if (max_n == 0) {
        /* initialise for n = 0 */
        a_arr = (seq_t*)malloc(sizeof(seq_t));
        if (!a_arr) return (seq_t)0;
        a_arr[0] = (seq_t)0;
        G = (uint16_t**)malloc(sizeof(uint16_t*));
        if (!G) return (seq_t)0;
        G[0] = NULL;
    }

    if (n > max_n) {
        /* grow a_arr to length n+1 */
        seq_t *new_a = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!new_a) return (seq_t)0;
        if (a_arr) memcpy(new_a, a_arr, (max_n + 1) * sizeof(seq_t));
        free(a_arr);
        a_arr = new_a;

        /* grow G pointer array */
        uint16_t **newG = (uint16_t**)realloc(G, (n + 1) * sizeof(uint16_t*));
        if (!newG) return (seq_t)0;
        G = newG;

        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            /* S[k] = G[cur‑k][k]  (1 ≤ k ≤ cur) */
            uint16_t *S = (uint16_t*)malloc((cur + 1) * sizeof(uint16_t));
            if (!S) return (seq_t)0;

            /* frequency of each Grundy value in S (values ≤ cur+1) */
            uint32_t *freq = (uint32_t*)malloc((cur + 3) * sizeof(uint32_t));
            if (!freq) return (seq_t)0;
            for (size_t i = 0; i < cur + 3; ++i) freq[i] = 0;

            for (idx_t k = 1; k <= cur; ++k) {
                idx_t x = cur - k;
                uint16_t val;
                if (k > x)          /* last removal larger than remainder */
                    val = (uint16_t)a_arr[x];
                else
                    val = G[x][k];
                S[k] = val;
                freq[val]++;
            }

            /* mex of the whole set S */
            uint16_t M = 0;
            while (freq[M] != 0) ++M;
            a_arr[cur] = (seq_t)M;

            /* compute G[cur][k] as mex of S without entry k */
            G[cur] = (uint16_t*)malloc((cur + 1) * sizeof(uint16_t));
            if (!G[cur]) return (seq_t)0;
            for (idx_t k = 1; k <= cur; ++k) {
                if (freq[S[k]] == 1 && S[k] < M)
                    G[cur][k] = S[k];
                else
                    G[cur][k] = M;
            }

            free(S);
            free(freq);
        }
        max_n = n;
    }

    return a_arr[n];
}
