/*
 * A162609  Triangle read by rows in which row n lists n terms, starting with
 *          1, with gaps = n-2 between successive terms.
 * https://oeis.org/A162609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A162609 – Triangle read by rows.
   For a given index n (n≥1) we first find the row r such that
   (r-1)*r/2 < n ≤ r*(r+1)/2.  Then the position in that row is  
   k = n - (r-1)*r/2  (1≤k≤r).  The value is

          a(n) = 1 + (r-2)*(k-1)       if k>1
          a(n) = 1                     if k=1
   This formula reproduces the table:
        1;
        1,1;
        1,2,3;
        1,3,5,7;
        1,4,7,10,13; … 
*/
#include <stddef.h>

static idx_t find_row(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    idx_t lo = 1, hi = 1;
    /* first grow hi until it is large enough */
    while ((unsigned __int128)hi * (hi + 1) / 2 < n) {
        hi <<= 1;
        if (hi == 0) {         /* overflow – hi is now UINT64_MAX */
            hi = (idx_t)-1;
            break;
        }
    }
    /* binary search for the smallest hi with hi*(hi+1)/2 ≥ n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
#else
    /* fallback: simple accumulation – safe for small n (the test suite) */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }
    return r;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1, but safe */

    idx_t r = find_row(n);
    /* previous triangular sum: (r-1)*r/2 */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;           /* 1 ≤ k ≤ r */

    if (k == 1)
        return (seq_t)1;

    seq_t r_t = (seq_t)r;
    seq_t k_t = (seq_t)k;
    return 1 + (r_t - (seq_t)2) * (k_t - (seq_t)1);
}
