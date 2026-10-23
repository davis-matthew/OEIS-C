/*
 * A175187  a(n) = A175186(n)+n.
 * https://oeis.org/A175187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A175186(n) + n where A175186 is defined recursively:
 *            A175186(1) = 0 If n is a power of two: A175186(n) = n-1
 *            Otherwise let m = 2^⌊log₂(n-1)⌋ (the greatest power of two < n)
 *            A175186(n) = A175186(n-m) + (n-m)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    idx_t orig = n;          /* original argument */
    seq_t sum = 0;           /* sum of remainders = A175186(n) */

    while (n > 0) {
        /* check if n is a power of two: n & (n-1) == 0 */
        if ((n & (n - 1)) == 0) {
            sum += (seq_t)(n - 1);   /* A175186(2^k) = 2^k - 1 */
            break;
        }

        /* largest power of two less than n: 2^floor_log2(n-1) */
        unsigned l = floor_log2_uint64(n - 1);
        idx_t m   = ((idx_t)1) << l;   /* m < n */

        sum += (seq_t)(n - m);   /* add remainder */
        n   -= m;                 /* recurse on the remainder */
    }

    return sum + (seq_t)orig;   /* A175187(n) = A175186(n) + n */
}
