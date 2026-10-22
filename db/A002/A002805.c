/*
 * A002805  a(n) = denominator of harmonic number H(n) = Sum_{i=1..n} 1/i.
 * https://oeis.org/A002805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, nice
 * Formula:   a(n) = lcm(1..n) / gcd(lcm(1..n), Sum_{k=1..n} lcm(1..n)/k); for
 *            each prime p with p^e || lcm(1..n), the exponent of p in a(n) is
 *            e - min(e, v_p(Sum_{k=1..n} lcm(1..n)/k)), and that p-adic
 *            valuation is found by working mod p^e
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 1;
    unsigned char *comp;
    uint64_t p;
    if (n < 2) return 1;
    comp = (unsigned char *)calloc((size_t)n + 1, 1);
    for (p = 2; p <= n; p++) {
        uint64_t q, num, den, k, m;
        int e, v, i;
        if (comp[p]) continue;
        if (p <= n / p)
            for (m = p * p; m <= n; m += p) comp[m] = 1;
        q = p; e = 1;
        while (q <= n / p) { q *= p; e++; }
        /* T = Sum_k p^(e - v_p(k)) / k' (k' = p-free part of k), as num/den mod q;
           den is a unit mod p, so v_p(T) = v_p(num) */
        num = 0; den = 1;
        for (k = 1; k <= n; k++) {
            uint64_t kk = k, t = q;
            while (kk % p == 0) { kk /= p; t /= p; }
            kk %= q;
            num = (num * kk + t * den) % q;
            den = (den * kk) % q;
        }
        v = 0;
        if (num == 0) v = e;
        else while (num % p == 0) { num /= p; v++; }
        for (i = v; i < e; i++) res *= (seq_t)p;
    }
    free(comp);
    return res;
}
