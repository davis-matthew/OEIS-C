/*
 * A174856  Square array read by antidiagonals up. Redheffer type matrix.
 *          T(1,1)=1 and T(n,1) = A049240.
 * https://oeis.org/A174856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element obtained by reading the infinite matrix
 *            T(i,j) ( i,j ≥ 1 ) by antidiagonals from bottom to top. The
 *            matrix T is defined as: T(1,j) = 1 T(i,1) = 0 if i is a perfect
 *            square > 1, otherwise 1 T(i,j>1) = 1 if j divides i, otherwise 0
 *            The sequence is the enumeration of T along...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    /* Integer square root: floor(sqrt(n)) using binary search. */
    idx_t low = 0, high = (idx_t)1 << 32;  /* high^2 ≥ 2^64-1 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static idx_t diag_of(idx_t n)
{
    /* Find the smallest s such that s*(s-1)/2 ≥ n. */
    idx_t low = 1, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate (i,j) corresponding to index n. */
    idx_t s = diag_of(n);                                   /* i+j = s */
    idx_t Cprev = (unsigned __int128)(s - 1) * (s - 2) / 2;  /* elements before diag s */
    idx_t t = n - Cprev - 1; /* offset within diag, from bottom (0-based) */
    idx_t i = s - 1 - t;
    idx_t j = t + 1;

    /* Compute T(i,j) with the rules above. */
    if (i == 1)
        return (seq_t)1;
    if (j == 1) {
        idx_t r = isqrt_u64(i);
        if (r * r == i && i > 1)
            return (seq_t)0;
        else
            return (seq_t)1;
    }
    return ((i % j) == 0) ? (seq_t)1 : (seq_t)0;
}
