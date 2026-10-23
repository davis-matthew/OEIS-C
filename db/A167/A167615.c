/*
 * A167615  Total number of positive integers below 10^n with 4 positive
 *          squares in their representation as sum of squares.
 * https://oeis.org/A167615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..k} ceil(10^n / 2^(2*i+3) - 7/8)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where k is the smallest index such that the term becomes zero. */
/* Implementation uses the equivalent integer form:
   a(n) = Sum_{k>=0} ( floor(A0 / 4^k) + ( (A0 % 4^k) > 7*4^k/8 ? 1 : 0 ) ),
   where A0 = floor(10^n / 8). */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* outside the offset, return 0 (undefined behaviour for sequence) */
        return 0;
    }

    /* Compute A0 = floor(10^n / 8).                 */
    /* For n < 3 the value fits in 64‑bit and we can
       compute it directly.                        */
    seq_t A0;
    if (n < 3) {
        seq_t pow10 = pow_seq((seq_t)10, (int)n); /* 10^n */
        A0 = pow10 / (seq_t)8;
    } else {
        /* For n >= 3, 10^n is divisible by 8, so
           10^n / 8 = 125 * 10^(n-3).              */
        seq_t pow10 = pow_seq((seq_t)10, (int)(n - 3));
        A0 = (seq_t)125 * pow10;
    }

    seq_t sum = 0;
    seq_t cur = A0;     /* floor(A0 / 4^k)  */
    seq_t p   = 1;      /* 4^k             */

    while (cur > 0) {
        seq_t r = A0 - cur * p;  /* A0 % 4^k */

        /* Threshold = floor(7 * 4^k / 8).  No overflow because 4^k
           is a power of four; only the term 4^k = 4 needs special
           handling (otherwise 4^k is divisible by 8).           */
        seq_t threshold;
        if (p <= 8) {                     /* p == 1 or 4       */
            threshold = (seq_t)(7 * p) / 8;
        } else {                          /* p is divisible by 8*/
            threshold = (seq_t)7 * (p / 8);
        }

        seq_t term = cur + (r > threshold ? 1 : 0);
        sum += term;

        cur = cur / 4;
        p   = p * 4;
    }

    return sum;
}
