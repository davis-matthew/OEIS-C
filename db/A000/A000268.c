/*
 * A000268  E.g.f.: -log(1+log(1+log(1-x))).
 * https://oeis.org/A000268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. -log(1+log(1+log(1-x))) = L(L(L(x))) with L(h) =
 *            -log(1-h). If g = L(h) then g' = h' + h*g', so in e.g.f.
 *            coefficients G(m) = H(m) + Sum_{k=1..m-1}
 *            binomial(m-1,k)*H(k)*G(m-k), m >= 1, applied three times
 *            starting from H = x (H(1) = 1, other H(m) = 0). a(n) = G3(n),
 *            with a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A268_MAXN 10002

static seq_t a268_L1[A268_MAXN];
static seq_t a268_L2[A268_MAXN];
static seq_t a268_L3[A268_MAXN];
static seq_t a268_P[A268_MAXN];   /* current Pascal row */
static idx_t a268_done = 0;       /* terms computed so far */

seq_t a(idx_t n)
{
    idx_t m, k;
    if (n < 1) return (seq_t)0;
    if (n > A268_MAXN - 2) n = A268_MAXN - 2;
    if (a268_done == 0) {
        a268_P[0] = 1;
    }
    while (a268_done < n) {
        m = a268_done + 1;
        /* bring the Pascal row from m-2 up to m-1 */
        if (m >= 2) {
            for (k = m - 1; k >= 1; k--)
                a268_P[k] += a268_P[k - 1];
        }
        /* level 1: L(x) */
        {
            seq_t s = (seq_t)(m == 1 ? 1 : 0);
            if (m >= 2) s += a268_P[1] * a268_L1[m - 1];
            a268_L1[m] = s;
        }
        /* level 2: L(L1) */
        {
            seq_t s = a268_L1[m];
            for (k = 1; k + 1 <= m; k++)
                s += a268_P[k] * a268_L1[k] * a268_L2[m - k];
            a268_L2[m] = s;
        }
        /* level 3: L(L2) */
        {
            seq_t s = a268_L2[m];
            for (k = 1; k + 1 <= m; k++)
                s += a268_P[k] * a268_L2[k] * a268_L3[m - k];
            a268_L3[m] = s;
        }
        a268_done = m;
    }
    return a268_L3[n];
}
