/*
 * A132119  A002260 + A000027 - A000012 as infinite lower triangular matrices.
 * https://oeis.org/A132119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula for A132119:
 *  Let r be the smallest integer with r(r+1)/2 ≥ n
 *  (i.e. r = ceil((√(8n+1)-1)/2)).
 *  Let c = n - r(r-1)/2   (1 ≤ c ≤ r).
 *  Then
 *      a(n) = r(r+1)/2            if c = 1 or c = r
 *      a(n) = r(r+1)/2 - (r - c)  otherwise.
 *
 *  This is exactly the row‑major flattening of the lower triangular
 *  matrix whose (r,c) entry is
 *      T(r)      if c = 1 or r,
 *      T(r) – (r-c)  otherwise,
 *  where T(r) = r(r+1)/2.
 *
 *  The implementation uses only integer arithmetic and works for any
 *  idx_t and seq_t types provided by the build.  128‑bit arithmetic is
 *  used only temporarily so that the results are correctly reduced
 *  modulo seq_t's width.
 */

#include <stdint.h>

/* find the row r such that r(r+1)/2 ≥ n (n ≥ 1) */
static idx_t row_of(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* grow hi until it is large enough */
    while ((unsigned __int128)hi * (hi + 1) / 2 < n) {
        hi *= 2;
    }
    /* binary search for the smallest r */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t r   = row_of(n);
    idx_t prev = (unsigned __int128)r * (r - 1) / 2;    /* triangular number of preceding rows */
    idx_t c    = n - prev;                              /* column within row r (1‑based) */

    unsigned __int128 T_full   = (unsigned __int128)r * (r + 1) / 2;  /* full T(r) */
    seq_t  T_seq = (seq_t)T_full;                                 /* reduce modulo seq_t */

    if (c == 1 || c == r)
        return T_seq;

    unsigned __int128 val_full = T_full - (unsigned __int128)(r - c);
    return (seq_t)val_full;
}
