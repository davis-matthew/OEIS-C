/*
 * A127775  Triangle read by rows: row n consists of n-1 zeros followed by
 *          2n-1.
 * https://oeis.org/A127775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if n is not a triangular number; a(n) = 2*k-1 where
 *            k(k+1)/2 = n
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Return the nth term of OEIS sequence A127775.
   idx_t and seq_t are defined by the surrounding build. */
seq_t a(idx_t n)
{
    /* For safety, reject indices before the offset (n < 1).  */
    if (n < 1)
        return (seq_t)0;

    /* Binary search for k such that k(k+1)/2 <= n.  */
    idx_t low  = 1;
    /* Upper bound: sqrt(2*n) + 2, computed in long double to avoid
       overflow when n is close to UINT64_MAX.  */
    idx_t high = (idx_t)(sqrtl((long double)2 * n) + 2);

    /* Ensure the upper bound is at least 1.  */
    if (high < 1)
        high = 1;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);          /* mid = (low + high) / 2 */
        __int128 val = (__int128)mid * (mid + 1) / 2;   /* compute mid*(mid+1)/2 safely */

        if (val <= ( __int128)n)
            low = mid + 1;
        else
            high = mid - 1;
    }

    /* After the loop, high is the largest k with k(k+1)/2 <= n.  */
    idx_t k = high;
    __int128 tri = (__int128)k * (k + 1) / 2;

    if (tri == ( __int128)n)
        return (seq_t)((seq_t)2 * k - 1);  /* non‑zero term */
    else
        return (seq_t)0;                   /* zero term */
}
