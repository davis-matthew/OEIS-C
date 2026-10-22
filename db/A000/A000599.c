/*
 * A000599  Number of secondary alcohols (alkanols or alkyl alcohols C_n
 *          H_{2n+1} OH) with n carbon atoms.
 * https://oeis.org/A000599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A(x) = x*(C(x)^2 + C(x^2))/2 with C(x) = B(x) - 1, where B(x)
 *            (A000598) satisfies B(x) = 1 + x*(B(x)^3 + 3*B(x)*B(x^2) +
 *            2*B(x^3))/6, b(0) = 1. Computed as: D = (B^2 + B(x^2))/2, b(m+1)
 *            = (B*D + B*B(x^2) + B(x^3))[x^m]/3, a(n) = D[n-1] - b(n-1) for n
 *            >= 1, a(0) = 0
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 50, u128 n <= 93,
 *            big any n
 * Helpers:   alloc_zero, half_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/half_tri.h"

static seq_t *Bv = 0, *Dv = 0;
static long long have = 0, cap = 0;
static int fixedw = -1;
static seq_t inv3;

static void extend(long long N)
{
    long long i, m, j;
    if (fixedw < 0) {
        seq_t mx = 0;
        mx -= 1;
        fixedw = (mx > 0) ? 1 : 0;
        if (fixedw) {
            /* all fixed widths are even, so 3 divides 2^w - 1 and -(2^w-1)/3 is 3^{-1} */
            seq_t q = mx / 3;
            inv3 = 0;
            inv3 -= q;
        }
    }
    if (N >= cap) {
        long long nc = 2 * N + 2;
        seq_t *nb, *nd;
        if (nc < 64) nc = 64;
        nb = alloc_zero((idx_t)nc);
        nd = alloc_zero((idx_t)nc);
        for (i = 0; i < have; i++) { nb[i] = Bv[i]; nd[i] = Dv[i]; }
        Bv = nb; Dv = nd; cap = nc;
    }
    if (have == 0) {
        Bv[0] = 1;
        Dv[0] = 1;
        have = 1;
    }
    while (have <= N) {
        seq_t s = 0;
        m = have - 1;
        for (i = 0; i <= m; i++) s += Bv[i] * Dv[m - i];
        for (i = 0; 2 * i <= m; i++) s += Bv[i] * Bv[m - 2 * i];
        if (m % 3 == 0) s += Bv[m / 3];
        if (fixedw) Bv[have] = s * inv3;
        else Bv[have] = s / 3;
        j = have;
        s = 0;
        for (i = 0; 2 * i < j; i++) s += Bv[i] * Bv[j - i];
        if (j % 2 == 0) s += half_tri(Bv[j / 2]);
        Dv[j] = s;
        have++;
    }
}

seq_t a(idx_t n)
{
    long long m;
    if (n == 0) return 0;
    m = (long long)(n - 1);
    extend(m);
    return Dv[m] - Bv[m];
}
