/*
 * A000662  Number of relations with 3 arguments on n nodes.
 * https://oeis.org/A000662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n} (fixA[s_1,
 *            s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...)) where fixA[s_1, s_2, ...]
 *            = 2^Sum_{i, j, k>=1} (i*j*k*s_i*s_j*s_k/lcm(i, j, k))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   gcdll, inv_odd_w, mulw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcdll.h"
#include "../utils/inv_odd_w.h"
#include "../utils/mulw.h"

static int w_bits = 0;              /* width of seq_t in bits, or -1 if arbitrary precision */
static long long *pd = 0, *pm = 0;  /* current partition: part sizes and multiplicities */
static int np = 0;
static seq_t acc;
static seq_t fact;

static void evaluate(void)
{
    long long c = 0;
    int a, b, d;
    for (a = 0; a < np; a++)
        for (b = 0; b < np; b++) {
            long long l1 = pd[a] / gcdll(pd[a], pd[b]) * pd[b];
            for (d = 0; d < np; d++) {
                long long l = l1 / gcdll(l1, pd[d]) * pd[d];
                c += pm[a] * pm[b] * pm[d] * ((pd[a] * pd[b] * pd[d]) / l);
            }
        }
    if (w_bits > 0) {
        long long v = 0, e;
        seq_t odd = 1, term;
        for (a = 0; a < np; a++) {
            long long t;
            for (t = 1; t <= pm[a]; t++) {
                long long x = pd[a], y = t;
                while ((x & 1) == 0) { x >>= 1; v++; }
                while ((y & 1) == 0) { y >>= 1; v++; }
                odd = mulw(odd, (seq_t)x);
                odd = mulw(odd, (seq_t)y);
            }
        }
        e = c - v;
        if (e >= w_bits) return;
        term = (seq_t)1 << e;
        term = mulw(term, inv_odd_w(odd));
        acc += term;
    } else {
        seq_t z = 1;
        long long t;
        for (a = 0; a < np; a++)
            for (t = 1; t <= pm[a]; t++) {
                z *= (seq_t)pd[a];
                z *= (seq_t)t;
            }
        acc += (fact / z) << c;
    }
}

static void rec(long long rem, long long maxp)
{
    long long p, m;
    if (rem == 0) {
        evaluate();
        return;
    }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        for (m = 1; m * p <= rem; m++) {
            pd[np] = p;
            pm[np] = m;
            np++;
            rec(rem - p * m, p - 1);
            np--;
        }
    }
}

seq_t a(idx_t n)
{
    long long N = (long long)n, i;
    seq_t res;
    if (w_bits == 0) {
        seq_t x = 1;
        int w = 0;
        while (x != 0 && w < 300) {
            x <<= 1;
            w++;
        }
        w_bits = (w >= 300) ? -1 : w;
    }
    pd = (long long *)malloc(sizeof(long long) * (size_t)(N + 1));
    pm = (long long *)malloc(sizeof(long long) * (size_t)(N + 1));
    np = 0;
    acc = 0;
    fact = 1;
    if (w_bits < 0)
        for (i = 2; i <= N; i++)
            fact *= (seq_t)i;
    rec(N, N);
    if (w_bits > 0)
        res = acc;
    else
        res = acc / fact;
    free(pd);
    free(pm);
    pd = 0;
    pm = 0;
    return res;
}
