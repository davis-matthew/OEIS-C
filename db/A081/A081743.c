/*
 * A081743  a(1)=1 then a(n)=a(n/2^k)+1 if n is even and 2^k is the largest
 *          power of 2 dividing n, a(n)=a(n-1) otherwise.
 * https://oeis.org/A081743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081743:
   a(1) = 1
   a(n) = a(n / 2^k) + 1 if n is even and 2^k is the largest power of 2 dividing n
   a(n) = a(n-1)        if n is odd
   This implementation evaluates the recurrence iteratively in O(log n) time. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t value = 1;           /* a(1) = 1 */
    while (n != 1) {
        if (n & 1) {           /* n is odd */
            n -= 1;           /* a(n) = a(n-1) */
        } else {               /* n is even */
            unsigned long long t = n;
            /* number of trailing zeros in n (largest k with 2^k|n) */
            unsigned int k = __builtin_ctzll(t);
            n >>= k;           /* divide by 2^k */
            value += 1;        /* a(n) = a(n/2^k) + 1 */
        }
    }
    return value;
}
