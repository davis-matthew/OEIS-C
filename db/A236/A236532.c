/*
 * A236532  Triangle T(n,k) read by rows: T(n,k) = floor(n*k/(n+k)), with 1 <=
 *          k <= n.
 * https://oeis.org/A236532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = floor(r * k / (r + k)), where r = least integer with
 *            r*(r+1)/2 >= n and k = n - r*(r-1)/2 (the column index in row r)
 * Range:     u8 n <= 208, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes the n-th term of OEIS sequence A236532.
   idx_t is an unsigned integer type (uint64_t).
   seq_t is an unsigned integer type or big integer type. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but return 0 */

    /* Binary search for the row r, 1 <= r <= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index k = n - (r-1)*r/2 */
    unsigned __int128 prev = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev;

    /* Compute T(r,k) = floor(r*k/(r+k)) */
    seq_t num = (seq_t)r * (seq_t)k;
    seq_t denom = (seq_t)r + (seq_t)k;
    seq_t res = num / denom;
    return res;
}
