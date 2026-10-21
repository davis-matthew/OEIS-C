/*
 * A087976  a(n) = A001221(A025586(n)), the number of distinct prime-factors
 *          of maximal term in 3x+1 iteration list started at n.
 * https://oeis.org/A087976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A025586(n)) – the number of distinct prime
 *            divisors of the maximum value reached in the Collatz sequence
 *            starting at n
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_u64.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t mx  = cur;        /* current maximum value */

    /* Collatz iteration */
    while (cur != 1) {
        if (cur & 1)                 /* odd */
            cur = cur * 3 + 1;
        else                         /* even */
            cur = cur >> 1;

        if (cur > mx)
            mx = cur;
    }

    /* Convert the maximum to a uint64_t for factoring.
       For all n in the intended range (up to 10 000 and beyond),
       the maximum value fits in 64 bits.        */
    uint64_t mx64 = (uint64_t)mx;
    return count_distinct_u64(mx64);
}
