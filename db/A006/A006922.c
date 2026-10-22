/*
 * A006922  Expansion of 1/eta(q)^24; Fourier coefficients of T_{14}.
 * https://oeis.org/A006922
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. x^(-1)*Product_{k>0} (1-x^k)^(-24) = 1/(E^3)^8 with E^3 =
 *            Sum_{k>=0} (-1)^k*(2k+1)*x^(k(k+1)/2) (Jacobi); h_0 = 1, h_j =
 *            h_{j-1}/E^3 via h_j[m] = h_{j-1}[m] + Sum_{k>=1}
 *            (-1)^(k+1)*(2k+1)*h_j[m-k(k+1)/2]; a(n) = h_8[n+1]
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 30, u128 n <= 91,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *h[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static idx_t cap = 0;   /* allocated length of each h[j] */
static idx_t done = 0;  /* number of computed coefficients */

seq_t a(idx_t n) {
    if (n < -1) return 0;
    idx_t idx = n + 1;
    if (idx >= done) {
        idx_t need = idx + 1;
        if (need > cap) {
            idx_t nc = 2 * cap;
            if (nc < need) nc = need;
            if (nc < 64) nc = 64;
            for (int j = 1; j <= 8; j++) h[j] = grow_zero(h[j], cap, nc);
            cap = nc;
        }
        for (idx_t m = done; m < need; m++) {
            for (int j = 1; j <= 8; j++) {
                seq_t pos = 0, neg = 0;
                if (j == 1) { if (m == 0) pos = 1; }
                else pos = h[j - 1][m];
                for (idx_t k = 1;; k++) {
                    idx_t t = k * (k + 1) / 2;
                    if (t > m) break;
                    if (k & 1) pos += h[j][m - t] * (2 * k + 1);
                    else neg += h[j][m - t] * (2 * k + 1);
                }
                h[j][m] = pos - neg;
            }
        }
        done = need;
    }
    return h[8][idx];
}
