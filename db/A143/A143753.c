/*
 * A143753  Irregular triangle: A120070 read downwards antidiagonals.
 * https://oeis.org/A143753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143753: Irregular triangle read by rows
 *
 * Row r (r ≥ 1) contains floor((r+1)/2) terms.
 * If r is even:   v(r,i) = 4·(r/2+1)·i    for i = 1 … r/2
 * If r is odd:    v(r,i) = (r+2)·(2i-1)   for i = 1 … (r+1)/2
 *
 * a(n) is the n‑th element of the concatenation of the rows.
 */

#include <stdint.h>

/* floor(sqrt(n)) on unsigned integers */
static inline idx_t isqrt(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= n / mid)      /* avoid overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Find the row r that contains position n */
    idx_t m = isqrt(n);               /* floor(sqrt(n)) */
    unsigned __int128 mm = (unsigned __int128)m * m;   /* m² */
    idx_t r;
    if (mm >= n) {
        r = 2 * m - 1;                /* r odd, T(r)=m² */
    } else if (mm + m >= n) {
        r = 2 * m;                    /* r even, T(r)=m(m+1) */
    } else {
        r = 2 * m + 1;                /* r odd, next block */
    }

    /* Row length */
    idx_t len = (r + 1) / 2;          /* ceil(r/2) */
    /* Total elements up to and including this row */
    unsigned __int128 T_r;
    if (r & 1) {                       /* r odd: T(r)=len² */
        unsigned __int128 k = (unsigned __int128)len;
        T_r = k * k;
    } else {                            /* r even: T(r)=len·(len+1) */
        unsigned __int128 k = (unsigned __int128)len;
        T_r = k * (k + 1);
    }
    idx_t prev_total = (idx_t)(T_r - len); /* elements before this row */
    idx_t offset = n - prev_total;         /* 1‑based index within row */

    /* Compute the term */
    if (r & 1) {                           /* odd row */
        seq_t first = (seq_t)r + 2;
        seq_t mult = (seq_t)(2 * (offset - 1) + 1); /* odd multiplier 1,3,5,... */
        return first * mult;
    } else {                               /* even row */
        seq_t k = (seq_t)len;
        return (seq_t)4 * (k + 1) * (seq_t)offset;
    }
}
