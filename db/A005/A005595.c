/*
 * A005595  States of a dynamic storage system.
 * https://oeis.org/A005595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (R(n) + N(n))/2, where R(n) = [x^n] (2 - 2*x + x^3)/((1 -
 *            x)*(1 - x - x^2)), i.e. R(0)=2, R(1)=2, R(2)=4, R(n) = 2*R(n-1)
 *            - R(n-3) + [n=3], and N(n) = (1/n) * Sum_{d|n} phi(n/d)*(L(2d) -
 *            1) with L = Lucas numbers, N(0) = 0 (this is the coefficient of
 *            x^n in Sum_{d>0} phi(d)*log((1-x^d)/(1-...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 96,
 *            big any n
 * Helpers:   phi_ull, rat_coef
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi_ull.h"
#include "../utils/rat_coef.h"

seq_t a(idx_t n) {
    long long m;
    seq_t pq, cq, Q, N, Rn, res;
    long long pr, cr, R;
    idx_t k;

    if (n == 0) return (seq_t)1;
    m = (long long)n;

    /* L(0) = 2, L(2) = 3, stored as q*m + r */
    pq = (seq_t)(2 / m); pr = 2 % m;
    cq = (seq_t)(3 / m); cr = 3 % m;

    Q = (seq_t)0;
    R = 0;
    for (k = 1; k <= n; k++) {
        if (k > 1) {
            long long t = 3 * cr - pr;
            long long c = (t >= 0) ? t / m : -1;
            long long nr = t - c * m;
            seq_t nq = cq * (seq_t)3;
            if (c >= 0) {
                nq += (seq_t)c;
                nq -= pq;
            } else {
                nq -= pq;
                nq -= (seq_t)1;
            }
            pq = cq; pr = cr;
            cq = nq; cr = nr;
        }
        if (n % k == 0) {
            long long ph = (long long)phi_ull((unsigned long long)(n / k));
            Q += cq * (seq_t)ph;
            R += ph * (cr - 1);
        }
    }
    /* R is divisible by m */
    if (R >= 0) Q += (seq_t)(R / m);
    else Q -= (seq_t)((-R) / m);
    N = Q;

    Rn = rat_coef(n);
    res = (Rn >> 1) + (N >> 1) + (((Rn & (seq_t)1) + (N & (seq_t)1)) >> 1);
    return res;
}
