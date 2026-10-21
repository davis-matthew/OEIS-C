/*
 * A061098  Concatenation of all numbers less than or equal to n other than
 *          the phi(n) numbers relatively prime to n.
 * https://oeis.org/A061098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of all integers k (1 ≤ k ≤ n) with gcd(k,n)
 *            > 1. Special case: a(1) = 1. For n > 1, a(n) =
 *            concat_{gcd(k,n)>1} k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static inline int num_digits_uint64(idx_t x) {
    int d = 1;
    while (x >= 10) {
        d++;
        x /= 10;
    }
    return d;
}

static inline seq_t pow10(int e) {
    seq_t v = 1;
    for (int i = 0; i < e; ++i) v *= 10;
    return v;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (gcd_idx(i, n) > 1) {
            int d = num_digits_uint64(i);
            seq_t mult = pow10(d);
            res = res * mult + (seq_t)i;
        }
    }
    return res;
}
