/*
 * A081493  Triangle T(n,k) read by rows in which the n-th row begins with
 *          T(n,1) = n and contains n terms of an arithmetic progression with
 *          a common difference of (n-1).
 * https://oeis.org/A081493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r + (r-1)*(j-1) where r = ceil((sqrt(8*n+1)-1)/2) (row
 *            number) j = n - r(r-1)/2 (position in row)
 * Range:     u8 n <= 151, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of an unsigned 128‑bit number
   returns floor(sqrt(x))  */
static idx_t isqrt_u128(unsigned __int128 x) {
    idx_t lo = 0;
    /* 2^34 ≈ 1.7e10 > sqrt(8·2^63+1) */
    idx_t hi = ((idx_t)1 << 34) - 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* Find row r : smallest r with r(r+1)/2 >= n */
    unsigned __int128 v = (unsigned __int128)n * 8 + 1;
    idx_t s = isqrt_u128(v);           /* floor(sqrt(8n+1)) */
    idx_t r = (s - 1) >> 1;            /* floor((sqrt-1)/2) */
    /* Adjust if needed */
    if ((unsigned __int128)r * (r + 1) / 2 < n) r++;

    /* offset of first element in this row */
    unsigned __int128 prod = (unsigned __int128)(r - 1) * r;
    idx_t offset = (idx_t)(prod >> 1);   /* (r-1)*r/2 */

    /* position within the row (1‑based) */
    idx_t j = n - offset;

    /* compute the term using seq_t arithmetic */
    seq_t rseq = (seq_t)r;
    seq_t jseq = (seq_t)j;
    return rseq + (rseq - (seq_t)1) * (jseq - (seq_t)1);
}
