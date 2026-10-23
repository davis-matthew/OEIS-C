/*
 * A183162  Least integer k such that floor(k*sqrt(n+1)) > k*sqrt(n).
 * https://oeis.org/A183162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that floor(k*sqrt(n+1)) >
 *            k*sqrt(n)
 * Range:     u8 n <= 8, u16 n <= 577, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic.  It
   contains a small integer‐square‐root routine that works
   for both fixed‐width integer types and arbitrary–precision
   seq_t classes.  The algorithm is
        k = 1;
        repeat
             k2  = k*k;
             v1  = k2*(n+1);
             v2  = k2*n;
             if floor_sqrt(v1) > floor_sqrt(v2) return k;
             k++;
        until found
   which directly encodes the definition of A183162.  */

#include <stdbool.h>
#include <stdint.h>

/*------------------------------------------------------------*/
/* integer square root: floor(sqrt(x))                         */
/* works for unsigned integer types as well as arbitrary-      */
/* precision seq_t classes that support the required operators */
/*------------------------------------------------------------*/
static inline seq_t isqrt_seq(seq_t x)
{
    if (x <= 1)
        return x;

    /* Find an upper bound high such that high*high > x */
    seq_t high = 1;
    while (high <= x / high) {
        high = high * 2;
    }

    /* Now binary search for the floor(sqrt(x)) */
    seq_t low = high / 2;
    while (low < high) {
        seq_t mid   = (low + high) / 2;
        seq_t mid2  = mid * mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/*-----------------------------------------------------------*/
/* Main sequence function                                   */
/*-----------------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;   /* promote index to seq_t */
    seq_t k  = 1;          /* start with k = 1 */

    for (;;) {
        seq_t k2 = k * k;
        seq_t v1 = k2 * (sn + 1);  /* (n+1)*k^2 */
        seq_t v2 = k2 * sn;        /*  n   *k^2 */

        seq_t L = isqrt_seq(v1);   /* floor(k*sqrt(n+1)) */
        seq_t R = isqrt_seq(v2);   /* floor(k*sqrt(n))   */

        if (L > R)
            return k;            /* condition satisfied */

        ++k;                     /* try next k */
    }
}
