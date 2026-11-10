/*
 * A077559  Final terms of rows of A077558.
 * https://oeis.org/A077559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n) of A077558, i.e. the n-th number >= n having the
 *            same prime signature as n. If n = p^e is a prime power, a(n) =
 *            q^e where q is the n-th prime >= p; otherwise found by a direct
 *            search
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool A077559_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static void A077559_sig(uint64_t m, uint8_t sig[64], uint8_t *len)
{
    uint8_t l = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            uint8_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            sig[l++] = e;
        }
    }
    if (t > 1) sig[l++] = 1;
    for (int i = 1; i < l; ++i) {
        uint8_t key = sig[i];
        int j = i - 1;
        while (j >= 0 && sig[j] > key) { sig[j + 1] = sig[j]; --j; }
        sig[j + 1] = key;
    }
    *len = l;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t t = (uint64_t)n, p = 0;
    for (uint64_t q = 2; q * q <= t; ++q) {
        if (t % q == 0) { p = q; break; }
    }
    if (p == 0) p = t;
    uint64_t u = t;
    unsigned e = 0;
    while (u % p == 0) { u /= p; ++e; }

    if (u == 1) {
        /* n = p^e: the n-th prime q >= p, then q^e */
        uint64_t q = p, count = 1;
        while (count < (uint64_t)n) {
            ++q;
            if (A077559_isprime(q)) ++count;
        }
        seq_t r = 1;
        for (unsigned i = 0; i < e; ++i) r *= (seq_t)q;
        return r;
    }

    uint8_t rs[64], rl;
    A077559_sig(t, rs, &rl);
    uint64_t count = 0;
    for (uint64_t v = t;; ++v) {
        uint8_t vs[64], vl;
        A077559_sig(v, vs, &vl);
        if (vl == rl && memcmp(rs, vs, rl) == 0) {
            if (++count == (uint64_t)n) return (seq_t)v;
        }
    }
}
