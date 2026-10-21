/*
 * A062775  Number of Pythagorean triples mod n: total number of solutions to
 *          x^2 + y^2 = z^2 mod n.
 * https://oeis.org/A062775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e: * if p == 2: * e
 *            odd (e = 2k−1): a(2^e) = 2^(3k−1) * (2^k − 1) * e even (e = 2k):
 *            a(2^e) = 2^(3k) * (2^(k+1) − 1) * if p is odd: * e odd (e =
 *            2k−1): a(p^e) = p^(3k−2) * (p^k + p^(k−1) − 1) * e even (e =
 *            2k): a(p^e) = p^(3k−1) * (p^(k+1)+ p^k − 1)
 * Range:     u8 n <= 15, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint_val.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* not used but safe */
    seq_t result = 1;
    idx_t m = n;
    /* factor n by simple trial division */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        seq_t apow;
        if (p == 2) {
            if (exp & 1) {               /* odd exponent: e = 2k-1 */
                idx_t k = (exp + 1) / 2;
                seq_t t1 = pow_uint_val((seq_t)2, 3 * k - 1);
                seq_t t2 = pow_uint_val((seq_t)2, k);
                apow = t1 * (t2 - 1);
            } else {                     /* even exponent: e = 2k */
                idx_t k = exp / 2;
                seq_t t1 = pow_uint_val((seq_t)2, 3 * k);
                seq_t t2 = pow_uint_val((seq_t)2, k + 1);
                apow = t1 * (t2 - 1);
            }
        } else {                       /* odd prime */
            if (exp & 1) {               /* odd exponent: e = 2k-1 */
                idx_t k = (exp + 1) / 2;
                seq_t t1 = pow_uint_val((seq_t)p, 3 * k - 2);
                seq_t t2 = pow_uint_val((seq_t)p, k);
                seq_t t3 = pow_uint_val((seq_t)p, k - 1);
                apow = t1 * (t2 + t3 - 1);
            } else {                     /* even exponent: e = 2k */
                idx_t k = exp / 2;
                seq_t t1 = pow_uint_val((seq_t)p, 3 * k - 1);
                seq_t t2 = pow_uint_val((seq_t)p, k + 1);
                seq_t t3 = pow_uint_val((seq_t)p, k);
                apow = t1 * (t2 + t3 - 1);
            }
        }
        result *= apow;
    }
    /* remaining prime factor (if any) */
    if (m > 1) {
        /* m is prime, exponent = 1 */
        idx_t p = m;
        seq_t apow;
        if (p == 2) {
            /* exponent = 1 = 2*1-1 => k=1 */
            seq_t t1 = pow_uint_val((seq_t)2, 3 * 1 - 1);   /* 2^2 = 4 */
            seq_t t2 = pow_uint_val((seq_t)2, 1);          /* 2^1 = 2 */
            apow = t1 * (t2 - 1);                          /* 4*(2-1)=4 */
        } else {
            /* exponent = 1 = 2*1-1 => k=1 */
            seq_t t1 = pow_uint_val((seq_t)p, 3 * 1 - 2);   /* p^1 = p */
            seq_t t2 = pow_uint_val((seq_t)p, 1);          /* p^1 = p */
            seq_t t3 = pow_uint_val((seq_t)p, 0);          /* p^0 = 1 */
            apow = t1 * (t2 + t3 - 1);                     /* p*(p+1-1)=p^2 */
        }
        result *= apow;
    }
    return result;
}
