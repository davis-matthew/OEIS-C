/*
 * A088965  Number of solutions to x^2 + 2y^2 == 1 (mod n).
 * https://oeis.org/A088965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = Number of solutions to x^2 + 2y^2 ≡ 1 (mod n). The
 *            sequence is multiplicative. For prime powers: a(2^e) = 2^e (e ≤
 *            2) a(2^e) = 2^(e+1) (e > 2) a(p^e) = (p-1)·p^(e-1) if p ≡ 1 or 3
 *            (mod 8) a(p^e) = (p+1)·p^(e-1) otherwise
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, int exp) {
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; undefined for 0 */
    seq_t res = 1;
    idx_t tmp = n;
    int e = 0;

    /* factor 2 */
    while ((tmp & 1) == 0) {
        ++e;
        tmp >>= 1;
    }
    if (e > 0) {
        seq_t factor;
        if (e <= 2)
            factor = ((seq_t)1) << e;       /* 2^e */
        else
            factor = ((seq_t)1) << (e + 1); /* 2^(e+1) */
        res *= factor;
    }

    /* odd factors */
    for (idx_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            e = 0;
            while (tmp % d == 0) {
                ++e;
                tmp /= d;
            }
            seq_t pe_minus1 = pow_seq_t((seq_t)d, e - 1); /* p^(e-1) */
            seq_t factor = ((d % 8 == 1 || d % 8 == 3) ?
                            ((seq_t)d - 1) : ((seq_t)d + 1));
            factor *= pe_minus1;
            res *= factor;
        }
    }

    /* remaining prime > sqrt(tmp) */
    if (tmp > 1) {
        idx_t p = tmp;
        seq_t factor = ((p % 8 == 1 || p % 8 == 3) ?
                        ((seq_t)p - 1) : ((seq_t)p + 1));
        res *= factor;  /* e = 1 => p^(e-1) = 1 */
    }

    return res;
}
