/*
 * A213042  Convolution of (1,0,2,0,3,0,...) and (1,0,0,2,0,0,3,0,0,...);
 *          i.e., (A027656(n)) and (A175676(n+2)).
 * https://oeis.org/A213042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k even, (n-k)%3==0} ((k/2)+1) * (((n-k)/3)+1)
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Convolution of (1,0,2,0,3,0,...) and (1,0,0,2,0,0,3,0,0,...) */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned long long sum = 0;
    idx_t max_i = n / 2;

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t k = 2 * i;
        idx_t m = n - k;          /* n-k is always >= 0 because k <= n */

        if (m % 3 == 0) {
            unsigned long long left  = (unsigned long long)(i + 1);
            unsigned long long right = (unsigned long long)(m / 3 + 1);
            sum += left * right;
        }
    }

    return (seq_t)sum;
}
