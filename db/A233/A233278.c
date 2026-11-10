/*
 * A233278  a(0)=0, a(1)=1, after which a(2n) = A055938(a(n)), a(2n+1) =
 *          A005187(1+a(n)).
 * https://oeis.org/A233278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0)=0, a(1)=1, a(2n) = A055938(a(n)), a(2n+1) =
 *            A005187(1+a(n)); A005187(k)=2k-popcount(k), A055938 = m-th
 *            non-term of A005187 found by binary search
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 83 terms through n = 7907 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned pc_seq(seq_t x) {
    unsigned c = 0;
    while (x > 0) { if ((x & 1) != 0) c++; x >>= 1; }
    return c;
}

/* A005187(k) = 2k - popcount(k) */
static seq_t A005187_f(seq_t k) {
    return 2 * k - (seq_t)pc_seq(k);
}

/* largest k with A005187_f(k) <= x */
static seq_t last_k(seq_t x) {
    seq_t lo = 0, hi = x;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (A005187_f(mid) <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

/* m-th positive integer not in A005187 (m >= 1) */
static seq_t A055938_a(seq_t m) {
    unsigned bits = 0;
    seq_t t = m;
    while (t > 0) { bits++; t >>= 1; }
    seq_t lo = 1, hi = 2 * m + (seq_t)bits + 8;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t missing = mid - last_k(mid);
        if (missing >= m) hi = mid; else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    int top = 63;
    while (((n >> top) & 1) == 0) top--;
    seq_t v = 1;
    for (int i = top - 1; i >= 0; i--) {
        if (((n >> i) & 1) == 0) v = A055938_a(v);
        else v = A005187_f(v + 1);
    }
    return v;
}
