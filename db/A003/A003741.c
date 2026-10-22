/*
 * A003741  Number of perfect matchings (or domino tilings) in O_5 X P_2n.
 * https://oeis.org/A003741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   transfer matrix. O_5 is K_5 minus one edge {0,1}. States are the
 *            subsets of column vertices already covered by a horizontal
 *            domino, grouped into orbits under the graph's symmetries. P maps
 *            even orbits to odd orbits and Q maps odd orbits to even orbits.
 *            M = P*Q, u_0 = e_0, u_k = M u_{k-1}, a(n) =...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/alloc_zero.h"

/* even orbits: 0 empty, 1 {0,1}, 2 {0,j}, 3 {j,k}, 4 size4 missing 0/1, 5 size4 missing j */
/* odd orbits:  0 {0}, 1 {j}, 2 {0,1,j}, 3 {0,j,k}, 4 {2,3,4}, 5 full */
static const int P[6][6] = {
    {6, 6, 3, 6, 0, 1},
    {0, 3, 0, 0, 1, 0},
    {1, 2, 0, 1, 0, 0},
    {2, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0}
};
static const int Q[6][6] = {
    {3, 0, 3, 3, 1, 0},
    {2, 1, 4, 0, 0, 1},
    {1, 0, 0, 1, 0, 0},
    {1, 0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0}
};

static long long M[6][6];
static seq_t u[6];
static seq_t *vals = NULL;
static idx_t cap = 0;
static idx_t cnt = 0;
static int inited = 0;

seq_t a(idx_t n) {
    int i, j, k;
    if (!inited) {
        for (i = 0; i < 6; i++)
            for (j = 0; j < 6; j++) {
                long long s = 0;
                for (k = 0; k < 6; k++) s += (long long)P[i][k] * Q[k][j];
                M[i][j] = s;
            }
        for (i = 0; i < 6; i++) u[i] = 0;
        u[0] = 1;
        inited = 1;
    }
    if (n + 1 > cap) {
        idx_t newcap = 2 * (n + 1);
        if (vals) vals = grow_zero(vals, cap, newcap);
        else vals = alloc_zero(newcap);
        cap = newcap;
    }
    if (cnt == 0) {
        vals[0] = u[0];
        cnt = 1;
    }
    while (cnt <= n) {
        seq_t t[6];
        for (i = 0; i < 6; i++) {
            t[i] = 0;
            for (j = 0; j < 6; j++)
                if (M[i][j]) t[i] += (seq_t)M[i][j] * u[j];
        }
        for (i = 0; i < 6; i++) u[i] = t[i];
        vals[cnt] = u[0];
        cnt++;
    }
    return vals[n];
}
