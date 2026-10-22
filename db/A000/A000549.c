/*
 * A000549  Numbers that are the sum of 2 squares but not sum of 3 nonzero
 *          squares.
 * https://oeis.org/A000549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   terms are the numbers 4^k*m, k >= 0, m in
 *            {1,2,5,10,13,25,37,58,85,130}, in sorted order (sums of 2
 *            squares that are not sums of 3 positive squares); a(n) =
 *            4*a(n-10) for n >= 22, and a(1..21) come from sorting the small
 *            4^k*m values
 * Range:     u8 n <= 25, u16 n <= 65, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t a549_base[10] = {1, 2, 5, 10, 13, 25, 37, 58, 85, 130};

static uint64_t a549_small(idx_t k) {
    /* k-th (1-based) smallest of 4^j*m, k <= 21 (all such values are <= 130*4) */
    uint64_t v[60];
    int cnt = 0, i, j;
    for (i = 0; i < 10; i++) {
        uint64_t x = a549_base[i];
        for (j = 0; j < 6; j++) {
            v[cnt++] = x;
            x *= 4;
        }
    }
    for (i = 1; i < cnt; i++) {
        uint64_t t = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > t) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = t;
    }
    return v[k - 1];
}

seq_t a(idx_t n) {
    idx_t q = 0;
    idx_t m = n;
    seq_t r;
    if (m > 21) {
        q = (m - 12) / 10;
        m -= 10 * q;
    }
    r = (seq_t)a549_small(m);
    while (q > 0) {
        r *= 4;
        q--;
    }
    return r;
}
