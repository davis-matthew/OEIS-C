/*
 * A064760  Variant of A002034 with initial term 0.
 * https://oeis.org/A064760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n >= 2, a(n) = smallest m such that n divides m!
 *            (Kempner numbers, A002034)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A064760_vp(seq_t m, seq_t p) {
    seq_t result = 0;
    seq_t power = p;
    while (power <= m) {
        result += m / power;
        if (power > m / p) break;
        power *= p;
    }
    return result;
}

static seq_t A064760_minm(seq_t p, seq_t k, seq_t upper_bound) {
    seq_t lo = 0, hi = upper_bound;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (A064760_vp(mid, p) >= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    if (n == 1) return 0;

    seq_t result = 0;
    seq_t temp = n;

    for (seq_t p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            seq_t k = 0;
            while (temp % p == 0) {
                k++;
                temp /= p;
            }
            seq_t m = A064760_minm(p, k, n);
            if (m > result) result = m;
        }
    }

    if (temp > 1) {
        if (temp > result) result = temp;
    }

    return result;
}
