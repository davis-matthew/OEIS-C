/*
 * A219463  Triangle read by rows: T(n,k) = 1 - A047999(n,k), 0 <= k <= n.
 * https://oeis.org/A219463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8384, u16 n <= 8384, u32 n <= 8384, u64 n <= 8384,
 *            u128 n <= 8384, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A219463
 *
 *  Triangle read by rows, the complement of Sierpinski's triangle:
 *    T(n,k) = 1 - (C(n,k) mod 2), 0 <= k <= n
 *
 *  The flattened sequence a(n) enumerates the rows of this triangle:
 *    n = r*(r+1)/2 + k   (0 <= k <= r)
 *
 *  a(n) = 1   if C(r,k) is even
 *  a(n) = 0   if C(r,k) is odd
 *
 *  The parity of C(r,k) is odd iff (k & ~r) == 0
 *  (i.e., every 1‑bit of k is also 1 in r – Lucas theorem).
 *
 *  Algorithm:
 *    • Find the row r by binary search on r*(r+1)/2 <= n
 *    • Compute k = n - r*(r+1)/2
 *    • Return 1 if (k & ~r) != 0 else 0
 *
 *  Complexity:  O(log n) time, constant memory.
 */

#include <stdint.h>   /* for the type widens used by __uint128_t if necessary */

seq_t a(idx_t n)
{
    /* --- step 1: locate the row r ------------------------------------------- */
    idx_t low = 0, high = n;          /* reasonable upper bound for r */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* compute mid*(mid+1)/2 safely in 128‑bit */
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n) {
            low = mid + 1;           /* mid is still feasible, search higher */
        } else {
            high = mid - 1;          /* mid too big, search lower */
        }
    }
    idx_t r = high;                   /* last mid that satisfied the condition */

    /* --- step 2: compute the column k --------------------------------------- */
    __uint128_t offset128 = (__uint128_t)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;  /* fits in idx_t because r ~ sqrt(n) */
    idx_t k = n - offset;             /* 0 <= k <= r */

    /* --- step 3: parity test and return ------------------------------------- */
    if ((k & (~r)) == 0)
        return (seq_t)0;              /* binomial coefficient is odd   */
    else
        return (seq_t)1;              /* binomial coefficient is even  */
}
