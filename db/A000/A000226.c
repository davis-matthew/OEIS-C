/*
 * A000226  Number of n-node unlabeled connected graphs with one cycle of
 *          length 3.
 * https://oeis.org/A000226
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of multisets of 3 rooted trees with n nodes in
 *            total = [x^n] (r(x)^3 + 3*r(x)*r(x^2) + 2*r(x^3))/6, where r(x)
 *            = Sum r(k) x^k is the g.f. of A000081: r(1) = 1, (m-1)*r(m) =
 *            Sum_{k=1..m-1} c(k)*r(m-k), c(k) = Sum_{d|k} d*r(d). Summed over
 *            size triples s1<=s2<=s3: r1*r2*r3, C(r+1,2)*r...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   a226_mc2, a226_mc3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a226_mc2.h"
#include "../utils/a226_mc3.h"

#define A226_MAXN 10010

static seq_t A226_R[A226_MAXN];
static seq_t A226_C[A226_MAXN];
static idx_t a226_have = 0;
static int a226_fixed = -1;

static void a226_detect(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 160; i++)
        x += x;
    a226_fixed = (x == 0) ? 1 : 0;
}

static void a226_extend(idx_t upto)
{
    while (a226_have < upto) {
        idx_t m = a226_have + 1;
        seq_t X = 0;
        if (m == 1) {
            X = 1;
        } else if (!a226_fixed) {
            seq_t S = 0;
            idx_t k;
            for (k = 1; k < m; k++)
                S += A226_C[k] * A226_R[m - k];
            X = S / (seq_t)(m - 1);
        } else {
            /* (m-1)*r(m) = Sum_d d*r(d)*T_d, T_d = Sum_{j>=1} r(m-j*d);
               split by quotient/remainder mod (m-1) so no term exceeds r(m) */
            unsigned long long mm = (unsigned long long)(m - 1);
            unsigned long long rem = 0;
            idx_t d;
            for (d = 1; d < m; d++) {
                seq_t T = 0;
                idx_t j;
                seq_t qa, qt;
                unsigned long long ra, rt, w;
                for (j = d; j < m; j += d)
                    T += A226_R[m - j];
                qa = A226_R[d] / (seq_t)mm;
                ra = (unsigned long long)(A226_R[d] % (seq_t)mm);
                X += (seq_t)d * qa * T;
                w = (unsigned long long)d * ra;
                qt = T / (seq_t)mm;
                rt = (unsigned long long)(T % (seq_t)mm);
                X += qt * (seq_t)w;
                rem += w * rt;
            }
            X += (seq_t)(rem / mm);
        }
        A226_R[m] = X;
        if (!a226_fixed) {
            seq_t c = 0;
            idx_t d;
            for (d = 1; d <= m; d++)
                if (m % d == 0)
                    c += A226_R[d] * (seq_t)d;
            A226_C[m] = c;
        }
        a226_have = m;
    }
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t s1, s2;
    if (n < 3) return 0;
    if (a226_fixed < 0) a226_detect();
    if (a226_fixed && n > 400) return 0;
    if (n >= A226_MAXN) return 0;
    a226_extend(n - 2);
    for (s1 = 1; 3 * s1 <= n; s1++) {
        for (s2 = s1; s1 + 2 * s2 <= n; s2++) {
            idx_t s3 = n - s1 - s2;
            if (s1 == s2 && s2 == s3)
                total += a226_mc3(A226_R[s1]);
            else if (s1 == s2)
                total += a226_mc2(A226_R[s1]) * A226_R[s3];
            else if (s2 == s3)
                total += A226_R[s1] * a226_mc2(A226_R[s2]);
            else
                total += A226_R[s1] * A226_R[s2] * A226_R[s3];
        }
    }
    return total;
}
