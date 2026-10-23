/*
 * A168605  Number of ways of partitioning the multiset {1,1,1,2,3,...,n-2}
 *          into exactly three nonempty parts.
 * https://oeis.org/A168605
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*3^(n-3) - 3*2^(n-2) + 3)/3 for n>=4, a(3)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n == 3)
        return (seq_t)1;
    if (n < 3)
        return (seq_t)0;   /* behavior for n<3 is unspecified */
    /* Use the equivalent simplified form to reduce intermediate size:
       a(n) = 5*3^(n-4) - 2^(n-2) + 1   (valid for n>=4) */
    return (seq_t)5 * pow3((idx_t)(n - 4)) - pow2((idx_t)(n - 2)) + (seq_t)1;
}
