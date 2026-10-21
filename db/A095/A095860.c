/*
 * A095860  Triangle read by rows: T(n,k) = n^(k^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) enumerates the triangle T(r,k)=r^(k^2) read row‑by‑row
 *            (offset 1). For a given index n (n>=1) the function finds the
 *            row r and colum c such that n = r(r+1)/2 - (r-1)r/2 + c and
 *            returns r^(c^2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 19, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes base^exp where base is seq_t and exp fits in idx_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find row r with r(r+1)/2 >= n via binary search. */
    idx_t low = 1, high = n;   /* high never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 cum = (__int128)mid * (mid + 1) / 2;
        if (cum < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Number of terms before row r: (r-1)*r/2 */
    __int128 before128 = (__int128)(r - 1) * r / 2;
    idx_t before = (idx_t)before128;

    /* Column c (1‑based) within the row */
    idx_t c = n - before;     /* 1 <= c <= r */

    seq_t base = (seq_t)r;
    idx_t exp = c * c;        /* c^2 fits in idx_t for all n considered */

    return pow_seq(base, exp);
}
