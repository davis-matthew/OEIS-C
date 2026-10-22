/*
 * A002550  Denominators of coefficients of log(1+x)/sqrt(1+x).
 * https://oeis.org/A002550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   log(1+x)/sqrt(1+x) = d/da (1+x)^a at a=-1/2, so |c(n)| =
 *            2*binomial(2n,n)/4^n * Sum_{j=0..n-1} 1/(2j+1). a(n) =
 *            denominator of c(n) = 4^n*L/gcd(...), where L =
 *            lcm(1,3,...,2n-1). The cancellation is found prime by prime with
 *            p-adic valuations: numerator = 2*C(2n,n)*P with P = Sum
 *            L/(2j+1), and v_...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   a2550_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2550_inv.h"

seq_t a(idx_t n) {
    uint64_t N = 2 * (uint64_t)n - 1;
    uint64_t q, i;
    seq_t r = 1;
    char *comp = (char *)calloc((size_t)N + 2, 1);

    /* 2-adic part */
    {
        uint64_t S = 0, x, tn;
        long long v2P = 0, v2C = 0, e2;
        for (q = 1; q <= N; q += 2) {
            int it;
            x = q;
            for (it = 0; it < 5; it++) x *= 2 - q * x;
            S += x;
        }
        if (S == 0) v2P = 64;
        else while ((S & 1) == 0) { S >>= 1; v2P++; }
        for (tn = n; tn; tn >>= 1) v2C += (long long)(tn & 1);
        e2 = 2 * (long long)n - 1 - v2C - v2P;
        for (; e2 > 0; e2--) r *= 2;
    }

    /* odd primes */
    for (i = 3; i <= N; i += 2) {
        uint64_t p, pe, T, t, pk;
        long long e, vC, vP, k;
        if (comp[i]) continue;
        for (q = i * i; q <= N; q += 2 * i) comp[q] = 1;
        p = i;
        pe = p; e = 1;
        while (pe * p <= N) { pe *= p; e++; }
        vC = 0;
        for (pk = p; pk <= 2 * (uint64_t)n; pk *= p) {
            vC += (long long)((2 * (uint64_t)n) / pk) - 2 * (long long)((uint64_t)n / pk);
            if (pk > (2 * (uint64_t)n) / p) break;
        }
        if (vC >= e) continue;
        T = 0;
        for (q = p; q <= N; q += 2 * p) {
            uint64_t u = q, pw = 1;
            long long v = 0, j;
            while (u % p == 0) { u /= p; v++; }
            for (j = 0; j < e - v; j++) pw *= p;
            T = (T + (pw % pe) * a2550_inv(u % pe, pe)) % pe;
        }
        vP = 0;
        if (T == 0) vP = e;
        else { t = T; while (t % p == 0) { t /= p; vP++; } }
        k = vC + vP;
        for (; k < e; k++) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
