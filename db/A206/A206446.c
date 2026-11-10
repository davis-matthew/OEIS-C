/*
 * A206446  Positions of 1 in A076478; complement of A206445.
 * https://oeis.org/A206446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (index i, 0-based, of the n-th 1 in A076478);
 *            computed by skipping blocks of L-bit strings (each holds
 *            L*2^(L-1) ones), then binary-searching with a popcount-sum count
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* number of ones in the L-bit strings 0..k-1 written in binary */
static idx_t A206446_ones_below(idx_t k, unsigned L)
{
    idx_t total = 0;
    for (unsigned j = 0; j < L && j < 64; ++j) {
        idx_t full = (j + 1 >= 64) ? 0 : (k >> (j + 1));
        idx_t part = (j + 1 >= 64) ? k : (k & (((idx_t)1 << (j + 1)) - 1));
        total += (full << j);
        if (part > ((idx_t)1 << j)) total += part - ((idx_t)1 << j);
    }
    return total;
}

seq_t a(idx_t n)
{
    idx_t rem = n;
    seq_t before = 0;
    unsigned L = 1;
    for (;; ++L) {
        if (L >= 60) break;
        idx_t blk = (idx_t)L * ((idx_t)1 << (L - 1));
        if (rem <= blk) break;
        rem -= blk;
        before += (seq_t)L * ((seq_t)1 << L);
    }
    /* smallest k with ones_below(k+1) >= rem */
    idx_t lo = 0, hi = ((idx_t)1 << L) - 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (A206446_ones_below(mid + 1, L) >= rem) hi = mid;
        else lo = mid + 1;
    }
    idx_t k = lo;
    idx_t r = rem - A206446_ones_below(k, L);
    unsigned t = 0;
    for (int j = (int)L - 1; j >= 0; --j, ++t) {
        if ((k >> j) & 1) {
            if (--r == 0) break;
        }
    }
    return before + (seq_t)k * (seq_t)L + (seq_t)t + 1;
}
