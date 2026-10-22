/*
 * A000016  a(n) is the number of distinct (infinite) output sequences from
 *          binary n-stage shift register which feeds back the complement of
 *          the last stage.
 * https://oeis.org/A000016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{odd d divides n} (phi(d)*2^(n/d))/(2*n), n>0; a(0) =
 *            1. With n = 2^k*m, m odd: a(n) = (Sum_{d|m} phi(d)*2^(n/d-k-1))
 *            / m. For fixed-width seq_t the exact division by odd m is done
 *            by multiplying by the inverse of m modulo 2^w, which is exact
 *            whenever a(n) fits
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   mul_s, seq_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_s.h"
#include "../utils/seq_bits.h"

static unsigned long long phi_ull(unsigned long long x)
{
    unsigned long long r = x, p;
    for (p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            r -= r / p;
        }
    }
    if (x > 1) r -= r / x;
    return r;
}

seq_t a(idx_t n)
{
    int bits;
    idx_t m, k, d;
    seq_t T = 0;

    if (n == 0) return 1;

    bits = seq_bits();
    m = n;
    k = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        k++;
    }

    for (d = 1; d <= m; d++) {
        idx_t e;
        seq_t pw;
        if (m % d != 0) continue;
        e = n / d - k - 1;
        if (bits >= 0 && e >= (idx_t)bits) continue; /* term is 0 mod 2^w */
        pw = (seq_t)1 << e;
        T += mul_s((seq_t)phi_ull((unsigned long long)d), pw);
    }

    if (bits < 0) {
        return T / (seq_t)m;
    } else {
        seq_t ms = (seq_t)m;
        seq_t inv = ms;
        int i;
        for (i = 0; i < 8; i++) {
            seq_t t = (seq_t)((seq_t)2 - mul_s(ms, inv));
            inv = mul_s(inv, t);
        }
        return mul_s(T, inv);
    }
}
