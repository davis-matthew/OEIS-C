/*
 * A069537  Multiples of 2 whose digit sum is 2.
 * https://oeis.org/A069537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n-th even integer whose decimal digit sum equals 2
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 46, u64 n <= 190, u128 n <= 742,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of 2·10^k and numbers 10^i+10^j with i>j≥1, sorted increasingly. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* First term */
    if (n == 1) {
        seq_t one = pow10(0);
        return one + one;                    /* 2 */
    }

    /* Find the block index m, where block m contains m numbers:
       10^m+10^j for j = 1..m-1, followed by 2·10^m.                 */
    idx_t t = n - 1;          /* offset after the first term         */
    idx_t m = 0;
    idx_t sum = 0;            /* sum of 1..m   ->   m(m+1)/2          */
    while (sum < t) {
        ++m;
        sum += m;
    }

    /* Index of the first element in block m (m ≥ 1) */
    idx_t startIndex = 2 + (m - 1) * m / 2;
    idx_t offset = n - startIndex;        /* 0 … m-1 */

    seq_t powM = pow10(m);
    if (offset < (m - 1)) {
        /* 10^m + 10^(offset+1) */
        idx_t j = offset + 1;
        return powM + pow10(j);
    } else {
        /* 2·10^m  */
        return powM + powM;
    }
}
