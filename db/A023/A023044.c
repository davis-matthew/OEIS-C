/*
 * A023044  7th differences of factorial numbers.
 * https://oeis.org/A023044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 21*n^6 + 196*n^5 + 1015*n^4 + 3094*n^3 + 5453*n^2
 *            + 5053*n + 1854)*n!
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    seq_t n6 = n5 * n1;
    seq_t n7 = n6 * n1;

    seq_t poly = n7
               + (seq_t)21   * n6
               + (seq_t)196  * n5
               + (seq_t)1015 * n4
               + (seq_t)3094 * n3
               + (seq_t)5453 * n2
               + (seq_t)5053 * n1
               + (seq_t)1854;

    return poly * factorial(n);
}
