/*
 * A061097  a(n) is the concatenation of the phi(n) numbers between 1 and n
 *          that are relatively prime to n.
 * https://oeis.org/A061097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of all k (1 ≤ k ≤ n) with gcd(k,n)=1 in
 *            decimal
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline int digits_u64(idx_t x) {
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

static inline seq_t pow10_seq(int d) {
    seq_t r = 1;
    while (d--) r *= 10;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset 1 in OEIS, but safe */
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd_u64(k, n) == 1) {
            int d = digits_u64(k);
            res = res * pow10_seq(d) + k;
        }
    }
    return res;
}
