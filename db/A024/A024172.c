/*
 * A024172  Integer part of ((3rd elementary symmetric function of
 *          1,2,..,n)/(2nd elementary symmetric function of 1,2,...,n)).
 * https://oeis.org/A024172
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (n-2)*n*(n+1) / (2*(3*n+2)) )
 * Range:     u8 n <= 40, u16 n <= 627, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;               /* offset is 2, values below are irrelevant */
    unsigned long long num = (unsigned long long)(n-2) * n * (n+1);
    unsigned long long den = 2 * (3*n + 2);
    unsigned long long res = num / den;   /* integer division gives floor */
    return (seq_t)res;
}
