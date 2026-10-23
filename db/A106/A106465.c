/*
 * A106465  Triangle read by rows, T(n, k) = 1 if n mod 2 = 1, otherwise (k +
 *          1) mod 2.
 * https://oeis.org/A106465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A106465 – Triangle read by rows
 *  T(n,k) = 1 if n is odd, otherwise (k+1) mod 2.
 *  The sequence a(n) is the triangle flattened in order:
 *      a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 *  In formula form:
 *      a(n) = 1            if row(n) is odd
 *           = ((col(n)+1) mod 2)   otherwise
 *  where row(n) = max r s.t. r(r+1)/2 ≤ n,
 *        col(n) = n - row(n)(row(n)+1)/2.
 *  (col(n) ≤ row(n)).
 */

#include <stdint.h>

/* Find the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
 * Uses binary search on an unsigned __int128 intermediate to avoid overflow.
 */
static idx_t
row_of(idx_t n)
{
    /* Expand an upper bound by doubling until it exceeds the target */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= (unsigned __int128)n) {
        if (high > (UINT64_MAX >> 1)) { /* avoid overflow of next shift */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search in [low, high] for the largest r with r(r+1)/2 <= n */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    idx_t r   = row_of(n);
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t col = n - tri;

    if (r & 1u)                /* odd row → all ones */
        return (seq_t)1;
    else                       /* even row → alternate 1,0 */
        return (seq_t)((col + 1u) & 1u);
}
