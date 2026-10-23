/*
 * A159797  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n, such that the difference between successive terms is equal
 *          to n-1.
 * https://oeis.org/A159797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = m + (n - m*(m+1)/2)*(m-1), where * m =
 *            floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 151, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS sequence A159797. */
seq_t a(idx_t n)
{
    /* 8*n + 1 fits in 128 bits for any 64‑bit n. */
    unsigned __int128 t = (unsigned __int128)n * 8 + 1;

    /* Integer square root of t by binary search on 0 … 2^34-1. */
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 34) - 1;          /* 2^34-1 ≈ 1.7e10 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;      /* ceil((low+high)/2) */
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= t)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;                            /* floor(sqrt(t)) */
    idx_t m = (s - 1) / 2;                    /* floor((sqrt(8*n+1)-1)/2) */

    /* n - m*(m+1)/2 */
    unsigned __int128 tri = (unsigned __int128)m * (m + 1) / 2;
    idx_t diff = n - (idx_t)tri;

    /* a(n) = m + diff * (m-1)  */
    unsigned __int128 ans128 = (unsigned __int128)m + (unsigned __int128)diff * (m - 1);

    return (seq_t)ans128;
}
