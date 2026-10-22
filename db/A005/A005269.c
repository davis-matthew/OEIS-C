/*
 * A005269  a(n) = number of length-n sequences s with s[1]=1, s[2]=1, s[k-1]
 *          <=s[k] <= s[k-2]+s[k-1] (s is called a sub-Fibonacci sequence of
 *          length n).
 * https://oeis.org/A005269
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f_{n-2}(1,1), where f_0(x,y) = 1 and f_k(x,y) =
 *            Sum_{t=0..x} f_{k-1}(y, y+t). f_k is kept as a polynomial in the
 *            basis C(x,i)*C(y-x,j), so that Sum_{t=0..x} C(t,j) = C(x+1,j+1).
 *            The new polynomial is re-expanded with Vandermonde and
 *            C(x,a)*C(x,b) = Sum_m C(m,a)*C(a,m-b)*C(x,m). The state i...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 13, u64 n <= 17, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *Cf = NULL, *Qf = NULL, *NCf = NULL, *PAS = NULL, *RES = NULL;
static idx_t Dm = 0;      /* stride / capacity of the square arrays */
static idx_t kdone = 0;   /* Cf holds f_kdone */

static void ensure(idx_t n) {
    idx_t newD, i, j;
    seq_t *nc, *nq, *nn, *np, *nr;
    if (n + 3 <= Dm) return;
    newD = n + 3;
    if (newD < 2 * Dm) newD = 2 * Dm;
    if (newD < 16) newD = 16;
    nc = alloc_zero(newD * newD);
    nq = alloc_zero(newD * newD);
    nn = alloc_zero(newD * newD);
    np = alloc_zero(newD * newD);
    nr = alloc_zero(newD + 2);
    for (i = 0; i < newD; i++) {
        np[i * newD] = 1;
        for (j = 1; j <= i; j++)
            np[i * newD + j] = np[(i - 1) * newD + j - 1] + np[(i - 1) * newD + j];
    }
    if (Dm == 0) {
        nc[0] = 1;
        nr[2] = 1;
        kdone = 0;
    } else {
        for (i = 0; i < Dm; i++)
            for (j = 0; j < Dm; j++)
                nc[i * newD + j] = Cf[i * Dm + j];
        for (i = 0; i < Dm + 2; i++) nr[i] = RES[i];
        release(Cf, Dm * Dm);
        release(Qf, Dm * Dm);
        release(NCf, Dm * Dm);
        release(PAS, Dm * Dm);
        release(RES, Dm + 2);
    }
    Cf = nc; Qf = nq; NCf = nn; PAS = np; RES = nr;
    Dm = newD;
}

static void step(void) {
    idx_t k = kdone, D = Dm;
    idx_t i, b, a, m, e, mlo;
    seq_t *tmp;
    for (i = 0; i <= k + 1; i++)
        for (b = 0; i + b <= k + 1; b++) {
            seq_t v = Cf[i * D + b];
            if (b > 0) v += Cf[i * D + b - 1];
            Qf[i * D + b] = v;
            NCf[i * D + b] = 0;
        }
    for (a = 0; a <= k + 1; a++) {
        for (b = 0; a + b <= k + 1; b++) {
            mlo = (a > b) ? a : b;
            for (m = mlo; m <= a + b; m++) {
                seq_t L = PAS[m * D + a] * PAS[a * D + (m - b)];
                for (e = 0; e + a + b <= k + 1; e++) {
                    seq_t qv = Qf[(e + a) * D + b];
                    if (qv == 0) continue;
                    NCf[m * D + e] += qv * L;
                }
            }
        }
    }
    tmp = Cf; Cf = NCf; NCf = tmp;
    kdone = k + 1;
    RES[kdone + 2] = Cf[0] + Cf[D];
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    ensure(n);
    while (kdone + 2 < n) step();
    return RES[n];
}
