/*
 * A233419  Least m such that A232637(m) is divisible by k, where k is the
 *          n-th number not divisible by 4; or 0 if no such m exists.
 * https://oeis.org/A233419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with k | A232637(m), k = n-th number not
 *            divisible by 4 (k = n + floor((n-1)/3)); A232637(m) =
 *            A000069(A000069(m)); computed by direct search over m
 * Range:     u8 n <= 11, u16 n <= 384, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9254 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* j-th odious number (j >= 1): of the pair {2t, 2t+1}, t=j-1, the one with odd popcount */
static seq_t A233419_odious(seq_t j)
{
    seq_t t = j - 1;
    seq_t x = t;
    bool p = false;
    while (x != 0) {
        if ((x & 1) != 0) p = !p;
        x >>= 1;
    }
    seq_t r = t * 2;
    if (!p) r += 1;
    return r;
}

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n + (seq_t)((n - 1) / 3);
    seq_t m = 1;
    for (;;) {
        seq_t v = A233419_odious(A233419_odious(m));
        if (v % k == 0) return m;
        ++m;
    }
}
