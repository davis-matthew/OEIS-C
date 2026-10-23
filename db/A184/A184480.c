/*
 * A184480  Lower s-Wythoff sequence, where s(n)=3n. Complement of A001956.
 * https://oeis.org/A184480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor[n*(-1+sqrt(13))/2]
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute it as a(n) = floor((floor(sqrt(13*n^2)) - n)/2)  */
/* using only integer arithmetic. */

#include <stdint.h>

/* Integer square root by binary search. Works for any seq_t that
   supports +,-,*,/,> etc., and does not overflow for the range
   used (13*n^2).  */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        /* mid*mid may overflow if seq_t is very small; guard with division */
        if (mid == 0 || mid <= x / mid) {
            /* mid^2 <= x -- try larger */

            /* Avoid overflow when computing mid+1 */
            seq_t next = mid + 1;
            if (next <= 0 || next > x / next) {
                /* mid is the largest integer with mid^2 <= x */
                return mid;
            }
            low = next;
        } else {
            if (mid == 0) return 0;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 return 0 */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;                 /* promote to seq_t */
    seq_t prod = nn * nn;                /* n^2 */
    prod *= (seq_t)13;                   /* 13*n^2 */
    seq_t sqrt_prod = isqrt_seq(prod);   /* floor( sqrt(13*n^2) ) = floor(n*sqrt13) */
    seq_t diff = sqrt_prod - nn;         /* floor(n*sqrt13) - n */
    return diff / (seq_t)2;              /* integer division truncates toward 0 for positive */
}
