/*
 * A005245  The (Mahler-Popken) complexity of n: minimal number of 1's
 *          required to build n using + and *.
 * https://oeis.org/A005245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look, changed
 * Formula:   dynamic-programming search: a(1) = 1, a(n) = min(
 *            min_{1<=i<=n/2} a(i)+a(n-i), min_{d|n, 1<d<n} a(d)+a(n/d) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static int *a005245_tab = NULL;
static idx_t a005245_done = 0;   /* tab[1..done] are computed */
static idx_t a005245_cap = 0;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > a005245_done) {
        if (n + 1 > a005245_cap) {
            idx_t nc = a005245_cap ? a005245_cap : 64;
            while (nc < n + 1) nc *= 2;
            int *nt = (int *)realloc(a005245_tab, (size_t)nc * sizeof(int));
            if (!nt) return (seq_t)0;
            a005245_tab = nt;
            a005245_cap = nc;
        }
        int *t = a005245_tab;
        if (a005245_done == 0) {
            t[0] = 0;
            t[1] = 1;
            a005245_done = 1;
        }
        for (idx_t m = a005245_done + 1; m <= n; m++) {
            int best = t[m - 1] + 1;
            for (idx_t i = 2; i <= m / 2; i++) {
                int c = t[i] + t[m - i];
                if (c < best) best = c;
            }
            for (idx_t d = 2; d * d <= m; d++) {
                if (m % d == 0) {
                    int c = t[d] + t[m / d];
                    if (c < best) best = c;
                }
            }
            t[m] = best;
        }
        a005245_done = n;
    }
    return (seq_t)a005245_tab[n];
}
