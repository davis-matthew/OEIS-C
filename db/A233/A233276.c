/*
 * A233276  a(0)=0, a(1)=1, after which a(2n) = A005187(1+a(n)), a(2n+1) =
 *          A055938(a(n)).
 * https://oeis.org/A233276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0)=0, a(1)=1, a(2n) = A005187(1+a(n)), a(2n+1) =
 *            A055938(a(n)), where A005187(k) = 2k - popcount(k) and
 *            A055938(m) = m + k - 1 for the smallest k with k - popcount(k)
 *            >= m. The code walks the binary digits of n from the top bit
 *            down
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 83 terms through n = 7830 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a_pop(seq_t x) {
    seq_t c = 0;
    while (x > 0) {
        if ((x & 1) != 0) c++;
        x >>= 1;
    }
    return c;
}

static seq_t A005187_a(seq_t x) {
    return 2 * x - a_pop(x);
}

static seq_t A055938_a(seq_t m) {
    seq_t bits = 0;
    seq_t t = m;
    while (t > 0) { bits++; t >>= 1; }
    seq_t lo = 0;
    seq_t hi = m + bits + 1;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid - a_pop(mid) >= m) hi = mid;
        else lo = mid + 1;
    }
    return m + lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    int top = 63;
    while (((n >> top) & 1) == 0) top--;
    seq_t v = 1;
    for (int i = top - 1; i >= 0; i--) {
        if (((n >> i) & 1) == 0) v = A005187_a(v + 1);
        else v = A055938_a(v);
    }
    return v;
}
