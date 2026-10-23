/*
 * A103128  a(n) = floor(sqrt(2n-1)).
 * https://oeis.org/A103128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(2*n-1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 1.
   Uses only integer arithmetic: no floating point or math libraries.
   The algorithm performs a binary search for the integer square root of
   2*n-1.  For the typical test range (n up to about 10000) this is
   extremely fast. */
seq_t a(idx_t n)
{
    /* For safety, handle n == 0, though the OEIS offset is 1. */
    if (n == 0) return (seq_t)0;

    /* m = 2*n - 1  (fits in idx_t for all relevant n). */
    idx_t m = n << 1;   /* multiply by 2 */
    m -= 1;

    /* Binary search for floor(sqrt(m)). */
    idx_t lo = 0, hi = m, root = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq <= m) {
            root = mid;
            lo   = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid underflow */
            hi   = mid - 1;
        }
    }

    return (seq_t)root;
}
