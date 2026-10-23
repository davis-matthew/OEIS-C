/*
 * A159368  Number of n X n arrays of squares of integers with every
 *          (n-1)X(n-1) subblock summing to 7.
 * https://oeis.org/A159368
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×n arrays of squares of integers with every
 *            (n–1)×(n–1) subblock summing to 7
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: binomial coefficient C(N,k) for small k (k ≤ 7)
   N is given as idx_t, k is int. */
static seq_t binom_u64(idx_t N, int k) {
    if (k > (int)N) return 0;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = (res * (seq_t)(N - k + i)) / (seq_t)i; /* exact division */
    }
    return res;
}

/* helper: number of ways to assign a cell block of size M
   with values 0,1,4 that sum to S (0 ≤ S ≤ 7). */
static seq_t ways_cells(idx_t M, int S) {
    switch (S) {
        case 0:   return 1;
        case 1:   return (seq_t)M;
        case 2:   return binom_u64(M, 2);
        case 3:   return binom_u64(M, 3);
        case 4:   return binom_u64(M, 4) + (seq_t)M;
        case 5:   return binom_u64(M, 5) + (seq_t)M * (seq_t)(M - 1);
        case 6:   return binom_u64(M, 6) + (seq_t)M * binom_u64(M - 1, 2);
        case 7:   return binom_u64(M, 7) + (seq_t)M * binom_u64(M - 1, 3);
        default:  return 0;
    }
}

seq_t a(idx_t n) {
    if (n < 3) return 0;               /* undefined below offset */

    idx_t edgeSize   = n - 2;          /* number of cells in an edge line */
    idx_t innerSize  = edgeSize * edgeSize; /* number of interior cells */

    /* pre‑compute ways for interior and edges */
    seq_t Wint[8];
    seq_t Wedge[8];
    seq_t Wcorner[8];

    for (int s = 0; s <= 7; ++s) {
        Wint[s]   = ways_cells(innerSize, s);
        Wedge[s]  = ways_cells(edgeSize, s);
        Wcorner[s] = ways_cells(1, s);
    }

    seq_t result = 0;

    for (idx_t ix = 0; ix < 8; ++ix) {            /* x = I  */
        for (idx_t iy = 0; iy < 8; ++iy) {        /* y = T  */
            for (idx_t iz = 0; iz < 8; ++iz) {    /* z = R  */
                for (idx_t iu = 0; iu < 8; ++iu) {/* u = U  */
                    for (idx_t iv = 0; iv < 8; ++iv) {/* v = V  */

                        int e = 7 - (int)ix - (int)iy - (int)iu;
                        if (e < 0 || e > 7) continue;
                        int f = 7 - (int)ix - (int)iy - (int)iv;
                        if (f < 0 || f > 7) continue;
                        int g = 7 - (int)ix - (int)iz - (int)iu;
                        if (g < 0 || g > 7) continue;
                        int h = 7 - (int)ix - (int)iz - (int)iv;
                        if (h < 0 || h > 7) continue;

                        /* corner contributions */
                        seq_t waysCorner = Wcorner[(idx_t)e] *
                                           Wcorner[(idx_t)f] *
                                           Wcorner[(idx_t)g] *
                                           Wcorner[(idx_t)h];
                        if (waysCorner == 0) continue;

                        /* ensure all edge/inner sums are reachable */
                        if (Wint[(idx_t)ix] == 0 ||
                            Wedge[(idx_t)iy] == 0 ||
                            Wedge[(idx_t)iz] == 0 ||
                            Wedge[(idx_t)iu] == 0 ||
                            Wedge[(idx_t)iv] == 0) continue;

                        seq_t ways = Wint[(idx_t)ix] *
                                     Wedge[(idx_t)iy] *
                                     Wedge[(idx_t)iz] *
                                     Wedge[(idx_t)iu] *
                                     Wedge[(idx_t)iv] *
                                     waysCorner;

                        result += ways;
                    }
                }
            }
        }
    }

    return result;
}
