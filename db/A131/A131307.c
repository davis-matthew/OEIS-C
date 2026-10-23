/*
 * A131307  (A127701 * A000012 + A000012(signed) * A127701) - A000012.
 * https://oeis.org/A131307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) = if k == r then 2*r-1 else r + (-1)^(r-k)*(k-1), where r
 *            is the unique integer with r*(r-1)/2 < n <= r*(r+1)/2 and k = n
 *            - r*(r-1)/2 (1‑based column in row r)
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, etc. */

seq_t a(idx_t n)
{
    /* --- find the row r ------------------------------- */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        seq_t t = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* --- column k within the row ---------------------- */
    seq_t triangular_prev = (seq_t)r * ((seq_t)r - 1) / 2;
    idx_t k = (idx_t)((seq_t)n - triangular_prev);   /* 1‑based */

    /* --- compute the entry of the triangle ----------- */
    if (k == r) {
        return (seq_t)(2 * r - 1);
    } else {
        int sign = ((r + k) & 1) ? -1 : 1;          /* (-1)^(r-k) */
        return (seq_t)r + (seq_t)sign * ((seq_t)k - 1);
    }
}
