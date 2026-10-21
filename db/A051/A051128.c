/*
 * A051128  Table T(n,k) = n^k read by upwards antidiagonals (n >= 1, k >= 1).
 * https://oeis.org/A051128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = (d - i + 1)^i, where d = ceil((sqrt(8*n+1)-1)/2) and i =
 *            n - d*(d-1)/2. The value is read from the table T(n,k)=n^k by
 *            upward antidiagonals
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 163, u64 n <= 423,
 *            u128 n <= 1112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Find diagonal d such that d*(d+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while (hi*(hi+1)/2 < n) hi <<= 1;         /* grow hi until large enough */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid*(mid+1)/2 >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t d = lo;                              /* diagonal length */

    /* previous diagonal's cumulative size */
    idx_t prev = (d-1)*d/2;
    idx_t i    = n - prev;                     /* position inside diagonal (1..d) */
    idx_t base_n = d - i + 1;                  /* row coordinate */

    return pow_seq_t((seq_t)base_n, (idx_t)i);  /* base^exponent */
}
