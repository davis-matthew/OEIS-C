/*
 * A189938  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sin(Pi/8), t=cos(Pi/8).
 * https://oeis.org/A189938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*csc(pi/8)) + floor(n*cot(pi/8))
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_nalpha, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_nalpha.h"

/* floor(n*sqrt(2))  using integer arithmetic */
static inline seq_t floor_nsqrt2(idx_t n)
{
    seq_t nsq = (seq_t)n * n;   /* n^2   */
    seq_t val = nsq * (seq_t)2; /* 2*n^2 */
    return floor_sqrt(val);
}

/* Main sequence function for A189938 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, guard against underflow */

    seq_t n_seq = (seq_t)n;
    seq_t alpha_floor = floor_nalpha(n);
    seq_t sqrt2_floor = floor_nsqrt2(n);

    /* a(n) = n + floor_nalpha + floor_nbeta
              = n + floor_nalpha + (n + floor_nsqrt2) */
    seq_t result = (seq_t)2 * n_seq + alpha_floor + sqrt2_floor;
    return result;
}
