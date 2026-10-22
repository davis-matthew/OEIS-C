/*
 * A002998  Smallest multiple of n whose digits sum to n.
 * https://oeis.org/A002998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   search, not a closed form. a(n) is the smallest number with
 *            digit sum n that is divisible by n. For digit lengths L =
 *            ceil(n/9), ceil(n/9)+1, ..., write each digit as 9 - e_i, so the
 *            e_i must add up to D = 9L - n. A DP over (value mod n, deficit
 *            used so far) finds the sets reachable from each suf...
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 79, u64 n <= 159,
 *            u128 n <= 339, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N, L, D, W, k, i, t, r, e;
    if (n == 0) return 0;
    N = (uint64_t)n;
    for (L = (N + 8) / 9; ; L++) {
        D = 9 * L - N;
        W = (D + 1) * N;
        uint64_t *pw = (uint64_t *)malloc(sizeof(uint64_t) * L);
        unsigned char *bits = (unsigned char *)calloc((size_t)((L * W) / 8 + 1), 1);
        unsigned char *cur = (unsigned char *)calloc((size_t)W, 1);
        unsigned char *nxt = (unsigned char *)calloc((size_t)W, 1);
        pw[0] = 1 % N;
        for (k = 1; k < L; k++) pw[k] = (pw[k - 1] * 10) % N;

        /* layer 0: empty suffix, remainder 0, deficit 0 */
        cur[0] = 1;
        bits[0] |= 1;
        for (k = 0; k + 1 < L; k++) {
            memset(nxt, 0, (size_t)W);
            for (t = 0; t <= D; t++) {
                const unsigned char *row = cur + t * N;
                for (r = 0; r < N; r++) {
                    if (!row[r]) continue;
                    for (e = 0; e <= 9 && t + e <= D; e++) {
                        uint64_t rr = (r + e * pw[k]) % N;
                        nxt[(t + e) * N + rr] = 1;
                    }
                }
            }
            {
                uint64_t base = (k + 1) * W, j;
                for (j = 0; j < W; j++)
                    if (nxt[j]) {
                        uint64_t idx = base + j;
                        bits[idx >> 3] |= (unsigned char)(1u << (idx & 7));
                    }
            }
            {
                unsigned char *tmp = cur; cur = nxt; nxt = tmp;
            }
        }

        /* need E = sum e_i 10^i == 10^L - 1 (mod N), sum e_i = D */
        {
            uint64_t C = ((pw[L - 1] * 10) % N + N - 1) % N;
            uint64_t R = C, T = D;
            seq_t v = 0;
            int ok = 1;
            for (i = L; i-- > 0; ) {
                uint64_t maxe = (i == L - 1) ? 8 : 9;
                int found = -1;
                uint64_t rr = 0;
                if (maxe > T) maxe = T;
                for (e = maxe + 1; e-- > 0; ) {
                    uint64_t sub = (e * pw[i]) % N;
                    uint64_t idx;
                    rr = (R + N - sub) % N;
                    idx = i * W + (T - e) * N + rr;
                    if (bits[idx >> 3] & (1u << (idx & 7))) { found = (int)e; break; }
                }
                if (found < 0) { ok = 0; break; }
                R = rr;
                T -= (uint64_t)found;
                v = v * 10 + (seq_t)(9 - found);
            }
            free(pw); free(bits); free(cur); free(nxt);
            if (ok) return v;
        }
    }
}
