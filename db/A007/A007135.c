/*
 * A007135  Number of symmetry sites in all planted 1,3-trees with 2n nodes.
 * https://oeis.org/A007135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   W(x) = x + (W(x)^2 + W(x^2))/2 (Wedderburn-Etherington numbers
 *            A001190), S(x) = S(x)*W(x) + S(x^2) + W(x^2), a(n) = [x^n] S(x);
 *            i.e. a(n) = Sum_{i=1..n-1} a(i)*W(n-i) + [n even]*(a(n/2) +
 *            W(n/2)), a(1) = 0
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *Wt = 0;
static seq_t *St = 0;
static idx_t have = 0; /* entries 0..have-1 computed */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n >= have) {
        idx_t newlen = n + 1;
        if (newlen < 2 * have) newlen = 2 * have;
        if (newlen < 3) newlen = 3;
        Wt = grow_zero(Wt, have, newlen);
        St = grow_zero(St, have, newlen);
        idx_t m;
        for (m = (have < 1 ? 1 : have); m < newlen; m++) {
            if (m == 1) {
                Wt[1] = (seq_t)1;
                St[1] = (seq_t)0;
                continue;
            }
            seq_t w = (seq_t)0;
            idx_t i;
            for (i = 1; 2 * i < m; i++)
                w += Wt[i] * Wt[m - i];
            if (m % 2 == 0) {
                seq_t h = Wt[m / 2];
                seq_t t;
                if (h % 2 == 0) t = (h / 2) * (h + 1);
                else t = h * ((h + 1) / 2);
                w += t;
            }
            Wt[m] = w;
            seq_t s = (seq_t)0;
            for (i = 1; i < m; i++)
                s += St[i] * Wt[m - i];
            if (m % 2 == 0)
                s += St[m / 2] + Wt[m / 2];
            St[m] = s;
        }
        have = newlen;
    }
    return St[n];
}
