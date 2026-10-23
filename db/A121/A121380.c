/*
 * A121380  Sums of primitive roots for n (or 0 if n has no primitive roots).
 * https://oeis.org/A121380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = sum of all primitive roots modulo n (0 if none exist)
 * Range:     u8 n <= 40, u16 n <= 502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd_uint64.h"

/* Greatest common divisor for uint64_t */

/* Euler totient φ(n) for uint64_t (works for n ≤ 10000 in this context) */
static inline uint64_t phi_uint64(uint64_t n) {
    uint64_t result = n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Determine whether n has primitive roots: n = 2, 4, p^k, or 2p^k (p odd) */
static inline bool has_primitive_root(uint64_t n) {
    if (n == 2 || n == 4) return true;
    if (n <= 1) return false;

    if (n % 2 == 0) {               /* 2·p^k case */
        uint64_t m = n / 2;
        if (m % 2 == 0) return false;        /* resulting p must be odd */
        uint64_t p = 0;
        for (uint64_t i = 3; i * i <= m; i += 2) {
            if (m % i == 0) { p = i; break; }
        }
        if (!p) p = m;
        uint64_t t = m;
        while (t % p == 0) t /= p;
        return t == 1;
    } else {                         /* odd n = p^k case */
        uint64_t p = 0;
        for (uint64_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) { p = i; break; }
        }
        if (!p) p = n;
        uint64_t t = n;
        while (t % p == 0) t /= p;
        return t == 1;
    }
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    if (N < 2) return 0;                   /* a(1) = 0 by definition */
    if (!has_primitive_root(N)) return 0;

    /* trivial case n = 2 */
    if (N == 2) return 1;                  /* only primitive root is 1 */

    uint64_t phi_n = phi_uint64(N);

    /* collect distinct prime factors of φ(n) */
    uint64_t pf[16];
    size_t pf_cnt = 0;
    uint64_t tmp = phi_n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            pf[pf_cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1 && pf_cnt < sizeof(pf)/sizeof(pf[0])) pf[pf_cnt++] = tmp;

    /* find one primitive root g modulo N */
    uint64_t g = 0;
    for (uint64_t cand = 2; cand < N; ++cand) {
        if (gcd_uint64(cand, N) != 1) continue;   /* must be a unit */
        bool ok = true;
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint64_t exp = phi_n / pf[i];
            if (mod_pow((seq_t)cand, exp, (seq_t)N) == 1) {
                ok = false; break;
            }
        }
        if (ok) { g = cand; break; }
    }
    if (g == 0) return 0;                    /* safety guard (shouldn't happen) */

    /* sum all primitive roots: g^k mod N for k coprime to φ(n) */
    seq_t sum = 0;
    for (uint64_t k = 1; k <= phi_n; ++k) {
        if (gcd_uint64(k, phi_n) == 1) {
            seq_t root = mod_pow((seq_t)g, k, (seq_t)N);
            sum += root;
        }
    }
    return sum;
}
