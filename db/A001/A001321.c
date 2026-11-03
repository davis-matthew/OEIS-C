/*
 * A001321  Number of equivalence classes of 3-valued Post functions of n
 *          variables under action of symmetric group S_n.
 * https://oeis.org/A001321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n!) * Sum_{sigma in S_n} 3^c(sigma), where c(sigma) is
 *            the number of cycles of sigma acting on {0,1,2}^n, i.e. for a
 *            cycle type with parts l_1..l_m and L = lcm(l_i): c = (1/L) *
 *            Sum_{k=0..L-1} 3^(Sum_i gcd(k,l_i)); grouped by cycle type, a(n)
 *            = Sum_{lambda |- n} 3^c(lambda) / z_lambda
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

static void a1321_term(const int *parts, int k, seq_t *Q, unsigned __int128 *R, uint64_t nf) {
    uint64_t z = 1, L = 1;
    int i, j;
    /* z_lambda = prod l^{m_l} * m_l! ; parts are non-increasing */
    i = 0;
    while (i < k) {
        int l = parts[i], m = 0;
        while (i < k && parts[i] == l) { m++; i++; }
        for (j = 1; j <= m; j++) z *= (uint64_t)l * (uint64_t)j;
    }
    for (i = 0; i < k; i++) L = L / gcd_u64(L, (uint64_t)parts[i]) * (uint64_t)parts[i];
    unsigned __int128 s = 0;
    uint64_t kk;
    for (kk = 0; kk < L; kk++) {
        uint64_t c = 0;
        for (i = 0; i < k; i++) c += kk == 0 ? (uint64_t)parts[i] : gcd_u64(kk, (uint64_t)parts[i]);
        unsigned __int128 p = 1;
        uint64_t t;
        for (t = 0; t < c; t++) p *= 3;
        s += p;
    }
    uint64_t e = (uint64_t)(s / L);
    /* compute 3^e = q*z + r without forming 3^e */
    seq_t q = (seq_t)(uint64_t)(1 / z);
    uint64_t r = 1 % z;
    int b;
    int top = 63;
    while (top >= 0 && !((e >> top) & 1)) top--;
    for (b = top; b >= 0; b--) {
        unsigned __int128 rr = (unsigned __int128)r * r;
        seq_t nq = q * q * (seq_t)z + q * (seq_t)r * 2 + (seq_t)(uint64_t)(rr / z);
        q = nq;
        r = (uint64_t)(rr % z);
        if ((e >> b) & 1) {
            unsigned __int128 t3 = (unsigned __int128)r * 3;
            q = q * 3 + (seq_t)(uint64_t)(t3 / z);
            r = (uint64_t)(t3 % z);
        }
    }
    *Q += q;
    *R += (unsigned __int128)r * (nf / z);
}

static void a1321_gen(int rem, int maxp, int *parts, int k, seq_t *Q, unsigned __int128 *R, uint64_t nf) {
    if (rem == 0) { a1321_term(parts, k, Q, R, nf); return; }
    int p;
    for (p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        parts[k] = p;
        a1321_gen(rem - p, p, parts, k + 1, Q, R, nf);
    }
}

seq_t a(idx_t n) {
    int nn = (int)n;
    uint64_t nf = 1;
    int i;
    for (i = 2; i <= nn; i++) nf *= (uint64_t)i;
    int *parts = (int *)malloc(sizeof(int) * (size_t)(nn + 1));
    seq_t Q = 0;
    unsigned __int128 R = 0;
    a1321_gen(nn, nn, parts, 0, &Q, &R, nf);
    free(parts);
    return Q + (seq_t)(uint64_t)(R / nf);
}
