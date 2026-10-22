/*
 * A007012  a(n) is number of k for which C(n,k) is not divisible by n.
 * https://oeis.org/A007012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : 0<=k<=n, n does not divide C(n,k)}; tested prime by
 *            prime: for each p^e || n, compare v_p(C(n,k)) = v_p(n!) -
 *            v_p(k!) - v_p((n-k)!) with e
 * Range:     u8 n <= 479, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t vfact(uint64_t m, uint64_t p) {
    uint64_t v = 0;
    while (m) {
        m /= p;
        v += m;
    }
    return v;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    uint64_t primes[64], exps[64], vn[64];
    int np = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            primes[np] = p;
            exps[np] = e;
            np++;
        }
    }
    if (m > 1) { primes[np] = m; exps[np] = 1; np++; }
    for (int i = 0; i < np; i++) vn[i] = vfact(n, primes[i]);

    uint64_t count = 0;
    for (uint64_t k = 0; k <= n; k++) {
        for (int i = 0; i < np; i++) {
            uint64_t v = vn[i] - vfact(k, primes[i]) - vfact(n - k, primes[i]);
            if (v < exps[i]) { count++; break; }
        }
    }
    return (seq_t)count;
}
