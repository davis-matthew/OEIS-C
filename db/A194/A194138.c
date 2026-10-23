/*
 * A194138  a(n) = Sum_{j=1..n} floor(j*(1+sqrt(2))), n-th partial sum of
 *          Beatty sequence for 1+sqrt(2).
 * https://oeis.org/A194138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1..n} floor(j*(1+sqrt(2)))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 400, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the identity floor(j*(1+sqrt(2))) = j + floor(j*sqrt(2)) */
/* and computes floor(j*sqrt(2)) as floor(sqrt(2*j*j)). */

#include <stdint.h>                 /* seq_t and idx_t are provided by the build */

static seq_t isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    while (low < high) {
        seq_t mid  = (low + high + 1) >> 1;   /* mid = ceil((low+high)/2) */
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t jj  = (seq_t)j * (seq_t)j;   /* j² */
        seq_t val = isqrt_seq(jj + jj);    /* floor(sqrt(2*j*j)) = floor(j*sqrt(2)) */
        sum += val + (seq_t)j;             /* add j + floor(j*sqrt(2)) */
    }
    return sum;
}
