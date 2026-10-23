/*
 * A120913  Triangle read by rows where t(n,m) is the (ceiling(n/m))th integer
 *          from among those positive integers not occurring earlier in the
 *          sequence.
 * https://oeis.org/A120913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r(r‑1)/2 + U(r,c) where * r = minimal integer such that
 *            r(r+1)/2 ≥ n, * c = n – r(r‑1)/2 (the column index in the row),
 *            * U(r,c) = ceil(r/c) if ceil(r/ceil(r/c)) = c, * otherwise
 *            U(r,c) = c
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)   /* defined for n ≥ 1 only */
        return 0;

    /* Find the row r (1‑based) and column c (1‑based) */
    idx_t low = 1, high = 1;
    /* grow high until high*(high+1)/2 ≥ n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* prefix sum of elements before row r */
    idx_t prefix = (idx_t)((unsigned __int128)r * (r - 1) / 2);

    /* column index in this row */
    idx_t c = n - prefix;  /* 1‑based */

    /* Compute U(r,c) */
    idx_t tmp = (r + c - 1) / c;            /* ceil(r/c)          */
    idx_t chk = (r + tmp - 1) / tmp;        /* ceil(r/tmp)        */
    idx_t u = (chk == c) ? tmp : c;

    /* Result */
    return (seq_t)(prefix + u);
}
