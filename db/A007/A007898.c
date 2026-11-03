/*
 * A007898  a(n) = psi_c(n), where Product_{k>1} 1/(1-1/k^s)^A007897(k) =
 *          Sum_{k>0} psi_c(k)/k^s.
 * https://oeis.org/A007898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Dirichlet Euler transform: Product_{k>1} 1/(1-1/k^s)^b(k) = Sum
 *            a(n)/n^s, where b = A007897 is multiplicative with b(p^e) =
 *            (phi(p^e) + #{x mod p^e : x^2 = 1})/2, i.e. b(p^e) =
 *            (phi(p^e)+2)/2 for odd p, b(2)=1, b(4)=2, b(2^e) = 2^(e-2)+2 for
 *            e>=3
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static idx_t b_of(idx_t k) {
    idx_t r = 1, m = k, p;
    if (m % 2 == 0) {
        int e = 0;
        idx_t phi = 1, s;
        while (m % 2 == 0) { m /= 2; e++; }
        for (int i = 1; i < e; i++) phi *= 2;
        s = (e == 1) ? 1 : (e == 2 ? 2 : 4);
        r *= (phi + s) / 2;
    }
    for (p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t phi = p - 1;
            m /= p;
            while (m % p == 0) { m /= p; phi *= p; }
            r *= (phi + 2) / 2;
        }
    }
    if (m > 1) r *= (m - 1 + 2) / 2;
    return r;
}

static seq_t *tab = NULL;
static idx_t cap = 0;

static void build(idx_t N) {
    seq_t *f = alloc_zero(N + 1);
    f[1] = 1;
    for (idx_t k = 2; k <= N; k++) {
        idx_t b = b_of(k);
        for (idx_t j = N / k; j >= 1; j--) {
            idx_t m = j * k;
            idx_t q = j, t = 1;
            seq_t c = 1;
            for (;;) {
                c = c * (seq_t)(b + t - 1) / (seq_t)t;
                f[m] += f[q] * c;
                if (q % k != 0) break;
                q /= k;
                t++;
            }
        }
    }
    tab = f;
    cap = N;
}

seq_t a(idx_t n) {
    if (n > cap) {
        idx_t N = n;
        if (N < 2 * cap) N = 2 * cap;
        if (N < 1024) N = 1024;
        if (tab != NULL) release(tab, cap + 1);
        tab = NULL;
        build(N);
    }
    return tab[n];
}
