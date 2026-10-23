/*
 * A122797  A P_3-stuttered arithmetic progression with a(n+1) = a(n) if n is
 *          a triangular number, a(n+1) = a(n) + 1 otherwise.
 * https://oeis.org/A122797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor((sqrt(8*(n-1)+1)-1)/2)
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count triangular numbers <= m: max k with k*(k+1)/2 <= m */
static idx_t triangular_count(idx_t m)
{
    /* For m up to 2^64-1 the answer is < 2^33, so we can use a
       binary search over [0, 1<<34] which guarantees convergence
       in < 35 steps. */
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 34) + 5;  /* safe upper bound */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* Compute mid*(mid+1)/2 without overflow using 128‑bit */
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod <= m)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n is 1‑based, offset 1.  Compute n - (#triangular ≤ n-1). */
    idx_t m = n - 1;
    idx_t tri = triangular_count(m);
    return (seq_t)(n - tri);
}
