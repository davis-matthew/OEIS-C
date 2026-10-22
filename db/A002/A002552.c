/*
 * A002552  Denominators of coefficients in Taylor series expansion of
 *          log(1+x)^2/sqrt(1+x).
 * https://oeis.org/A002552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (-1)^n*binomial(2n,n)*2^(3-2n)*e2(n),
 *            where e2(n) = Sum_{0<=i<j<n} 1/((2i+1)(2j+1)); this is the
 *            coefficient of x^n in log(1+x)^2/sqrt(1+x) (second s-derivative
 *            of binomial(s,n) at s=-1/2). The denominator is built prime by
 *            prime from p-adic valuations: Legendre's formula fo...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   a2552_modinv, a2552_spow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/a2552_modinv.h"
#include "../utils/a2552_spow.h"

seq_t a(idx_t n) {
    if (n < 2) return 1;
    long long N = (long long)n;
    seq_t res = 1;

    /* 2-adic part: e2 modulo 2^64 using inverses of odd numbers */
    {
        uint64_t O = 0, E = 0;
        long long j;
        for (j = 0; j < N; j++) {
            uint64_t m = 2 * (uint64_t)j + 1, x = m;
            int k;
            for (k = 0; k < 5; k++) x *= 2 - m * x;
            E += O * x;
            O += x;
        }
        long long v2e = 0;
        if (E == 0) v2e = 64;
        else while (!(E & 1)) { E >>= 1; v2e++; }
        long long pc = 0;
        uint64_t t = (uint64_t)n;
        while (t) { pc += (long long)(t & 1); t >>= 1; }
        long long tot = pc + 3 - 2 * N + v2e;
        if (tot < 0) res = res * a2552_spow((seq_t)2, -tot);
    }

    /* odd primes p <= n (primes p > n never divide the denominator) */
    char *comp = (char *)calloc((size_t)N + 1, 1);
    uint64_t *pre = (uint64_t *)malloc((size_t)N * sizeof(uint64_t));
    uint64_t *uarr = (uint64_t *)malloc((size_t)N * sizeof(uint64_t));
    unsigned char *varr = (unsigned char *)malloc((size_t)N);
    long long i, j;
    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = 1;

    for (long long pl = 3; pl <= N; pl += 2) {
        if (comp[pl]) continue;
        uint64_t p = (uint64_t)pl;
        uint64_t lim = 2 * (uint64_t)N - 1;
        uint64_t pw = 1, ppow[64];
        int s = 0;
        ppow[0] = 1;
        while (pw * p <= lim) { pw *= p; s++; ppow[s] = pw; }
        uint64_t q = pw * pw;

        uint64_t prod = 1, r = 1 % p;
        for (j = 0; j < N; j++) {
            uint64_t u = 2 * (uint64_t)j + 1;
            unsigned char v = 0;
            if (r == 0) {
                while (u % p == 0) { u /= p; v++; }
            }
            uarr[j] = u % q;
            varr[j] = v;
            prod = prod * uarr[j] % q;
            pre[j] = prod;
            r += 2;
            if (r >= p) r -= p;
        }
        uint64_t inv = a2552_modinv(prod, q);
        uint64_t O = 0, E = 0;
        for (j = N - 1; j >= 0; j--) {
            uint64_t invu = (j > 0) ? inv * pre[j - 1] % q : inv;
            inv = inv * uarr[j] % q;
            uint64_t t = ppow[s - varr[j]] * invu % q;
            E = (E + O * t) % q;
            O = (O + t) % q;
        }
        if (E == 0) continue;
        long long vS = 0;
        while (E % p == 0) { E /= p; vS++; }
        long long vC = 0;
        uint64_t pk = p;
        while (pk <= 2 * (uint64_t)N) {
            vC += (long long)((2 * (uint64_t)N) / pk - 2 * ((uint64_t)N / pk));
            if (pk > (2 * (uint64_t)N) / p) break;
            pk *= p;
        }
        long long ex = 2 * (long long)s - vS - vC;
        if (ex > 0) res = res * a2552_spow((seq_t)p, ex);
    }

    free(comp);
    free(pre);
    free(uarr);
    free(varr);
    return res;
}
