/*
 * A050321  k such that A050292(k) is different from A004396(k).
 * https://oeis.org/A050321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (k>=0, in increasing order) with A050292(k) !=
 *            A004396(k); A050292(k) = k - A050292(floor(k/2)), A004396(k) =
 *            floor((2k+1)/3); found by linear search
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A050292_a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    int odd = 0;
    while (n > 0) {
        if (odd) neg += (seq_t)n; else pos += (seq_t)n;
        odd = !odd;
        n >>= 1;
    }
    return pos - neg;
}

static seq_t A004396_a(idx_t n)
{
    return ((seq_t)n * 2 + 1) / 3;
}

seq_t a(idx_t n)
{
    idx_t k = 0;
    idx_t cnt = 0;
    for (;; k++) {
        if (A050292_a(k) != A004396_a(k)) {
            if (cnt == n) return (seq_t)k;
            cnt++;
        }
    }
}
