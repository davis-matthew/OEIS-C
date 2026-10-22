/*
 * A001904  From higher order Bernoulli numbers: absolute value of numerator
 *          of D Number D2n(2n).
 * https://oeis.org/A001904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = |numerator(D(2n))| with D(2n) = Sum_{k=0..n}
 *            t(2n+1,2k+1)/(2k+1), where Sum_k t(2n+1,2k+1) z^k =
 *            Product_{i=1..n} (z - (2i-1)^2) (central factorial numbers of
 *            the first kind; from the e.g.f. x/(arcsinh(x)*sqrt(1+x^2)) via x
 *            = sinh(t)). The fraction is reduced by gcd with
 *            lcm(1,3,5,...,2n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   a001028_grow, a001028_z, a1904_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"
#include "../utils/a1904_fixed.h"

static seq_t *a1904_Q = NULL;   /* coefficients of Prod_{i=1..cnt} (z + (2i-1)^2) */
static idx_t a1904_cnt = 0;
static idx_t a1904_cap = 0;

static void a1904_extend(idx_t n) {
    if (a1904_Q == NULL) {
        a1904_cap = n + 16;
        a1904_Q = a001028_z(a1904_cap);
        a1904_Q[0] = 1;
        a1904_cnt = 0;
    }
    if (n + 1 > a1904_cap) {
        idx_t nc = 2 * (n + 1);
        a1904_Q = a001028_grow(a1904_Q, a1904_cap, nc);
        a1904_cap = nc;
    }
    while (a1904_cnt < n) {
        idx_t m = a1904_cnt;
        idx_t o = 2 * m + 1;
        seq_t aa = (seq_t)o * (seq_t)o;
        idx_t j;
        a1904_Q[m + 1] = a1904_Q[m];
        for (j = m; j >= 1; j--)
            a1904_Q[j] = a1904_Q[j - 1] + aa * a1904_Q[j];
        a1904_Q[0] = aa * a1904_Q[0];
        a1904_cnt++;
    }
}

seq_t a(idx_t n) {
    idx_t top = 2 * n + 1;
    idx_t i, k, np = 0;
    unsigned char *comp;
    idx_t *pr, *pe;
    int *ex;
    int fixed = a1904_fixed();
    seq_t result;

    if (n == 0) return 1;
    if (fixed && n > 200) return 0; /* cannot fit in a fixed-width type anyway */

    a1904_extend(n);

    comp = (unsigned char *)calloc(top + 1, 1);
    pr = (idx_t *)malloc((top + 1) * sizeof(idx_t));
    pe = (idx_t *)malloc((top + 1) * sizeof(idx_t));
    ex = (int *)malloc((top + 1) * sizeof(int));
    for (i = 3; i <= top; i += 2) {
        if (comp[i]) continue;
        {
            idx_t j;
            for (j = i * i; j <= top; j += 2 * i) comp[j] = 1;
        }
        {
            idx_t q = i;
            int e = 1;
            while (q * i <= top) { q *= i; e++; }
            pr[np] = i; pe[np] = q; ex[np] = e; np++;
        }
    }

    if (!fixed) {
        seq_t L = 1, pos = 0, neg = 0, S;
        for (i = 0; i < np; i++) L *= pe[i];
        for (k = 0; k <= n; k++) {
            seq_t t = a1904_Q[k] * (L / (seq_t)(2 * k + 1));
            if (((n - k) & 1) == 0) pos += t; else neg += t;
        }
        if (pos >= neg) S = pos - neg; else S = neg - pos;
        for (i = 0; i < np; i++) {
            int c = 0;
            while (c < ex[i] && S % pr[i] == 0) { S /= pr[i]; c++; }
        }
        result = S;
    } else {
        seq_t Sw = 0, g = 1, x;
        uint64_t *R = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        int it;
        for (k = 0; k <= n; k++) {
            idx_t d = 2 * k + 1;
            seq_t Lk = 1, t;
            for (i = 0; i < np; i++) {
                idx_t dd = d;
                int v = 0, r;
                while (dd % pr[i] == 0) { dd /= pr[i]; v++; }
                for (r = v; r < ex[i]; r++) Lk *= pr[i];
            }
            t = a1904_Q[k] * Lk;
            if (((n - k) & 1) == 0) Sw += t; else Sw -= t;
        }
        for (i = 0; i < np; i++) {
            uint64_t p = pr[i], q = pe[i], Mo = 1, sum = 0;
            idx_t m, j;
            int v, r;
            R[0] = 1 % q;
            for (m = 0; m < n; m++) {
                uint64_t o = (2 * m + 1) % q;
                uint64_t aa = o * o % q;
                R[m + 1] = R[m];
                for (j = m; j >= 1; j--) R[j] = (R[j - 1] + aa * R[j]) % q;
                R[0] = aa * R[0] % q;
            }
            for (j = 0; j < np; j++)
                if (j != i) Mo = Mo * (pe[j] % q) % q;
            for (k = 0; k <= n; k++) {
                uint64_t d = 2 * k + 1, pk = 1, term;
                int vd = 0;
                while (d % p == 0) { d /= p; vd++; }
                for (r = vd; r < ex[i]; r++) pk = pk * p % q;
                term = R[k] * Mo % q * pk % q;
                if (((n - k) & 1) == 0) sum = (sum + term) % q;
                else sum = (sum + q - term) % q;
            }
            if (sum == 0) v = ex[i];
            else { v = 0; while (sum % p == 0) { sum /= p; v++; } }
            for (r = 0; r < v; r++) g *= (seq_t)p;
        }
        free(R);
        x = g;
        for (it = 0; it < 8; it++) x = x * ((seq_t)2 - g * x);
        if (n & 1) Sw = (seq_t)0 - Sw;
        result = Sw * x;
    }

    free(comp); free(pr); free(pe); free(ex);
    return result;
}
