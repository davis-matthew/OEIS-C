/*
 * A075559  Smallest multiple of n not equal to n ending in (digits of) n.
 * https://oeis.org/A075559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n * (1 + 10^d / gcd(n,10^d)), where d = number of decimal
 *            digits of n
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   pow10_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_d.h"
static inline idx_t num_digits(idx_t n) {
    idx_t d = 0;
    do {
        ++d;
        n /= 10;
    } while (n != 0);
    return d;
}

static inline seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* offset starts at 1, but safeguard */
    seq_t sn = (seq_t)n;
    idx_t d = num_digits(n);
    seq_t ten = pow10_d(d);
    seq_t g = gcd_seq_t(sn, ten);
    seq_t factor = ten / g;      /* 10^d / gcd(n,10^d) */
    seq_t k = factor + 1;        /* minimal multiplier >1 */
    return sn * k;
}
