/*
 * A067587  Inverse of A066884 considered as a permutation of the positive
 *          integers.
 * https://oeis.org/A067587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = binomial(w(n)+p(n),2) + p(n) + 1 * where w(n) =
 *            popcount(n) and p(n) = #{ m < n | popcount(m) = w(n) }
 * Range:     u8 n <= 66, u16 n <= 1410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9256 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute C(n,k) for small n (n <= 63) as seq_t */
static seq_t binom_small(int n, int k) {
    if (k < 0 || k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (int i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / (seq_t)i;
    }
    return res;
}

/* count numbers < n with popcount == w (w is integer >=0) */
static seq_t count_lt(idx_t n, unsigned int w) {
    seq_t count = (seq_t)0;
    /* msb index of n (n >= 1) */
    int bits = 63 - __builtin_clzll((uint64_t)n);
    int w_remaining = w;
    for (int i = bits; i >= 0; --i) {
        if ((n >> i) & 1ULL) {
            if (w_remaining > 0)
                count += binom_small(i, w_remaining);
            --w_remaining;
            if (w_remaining < 0) break;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    unsigned int w = __builtin_popcountll((uint64_t)n);
    seq_t p = count_lt(n, w);
    seq_t total = (seq_t)w + p;               /* w + p */
    seq_t a_val = total * (total - (seq_t)1) / (seq_t)2 + p + (seq_t)1;
    return a_val;
}
