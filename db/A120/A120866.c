/*
 * A120866  a(n) is the number j for which there exists a unique pair (j,k) of
 *          positive integers such that (j + k + 1)^2 - 4*k = 20*n^2.
 * https://oeis.org/A120866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n^2 - floor(n*sqrt(5))^2
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 5498, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(n*sqrt(5)) by taking the integer square root of 5*n^2.   */
/* The result is valid for all n >= 1.                                       */
#include <stdint.h>
#include <stdbool.h>

/* integer square root of v (floor(sqrt(v))) using binary search.           */
/* Works for any seq_t that supports arithmetic operators.                  */
static inline seq_t isqrt_seq(seq_t v)
{
    seq_t low = 0;
    /* 3*n is an upper bound for sqrt(5)*n when n is in idx_t.               */
    /* For very large n that would overflow 64-bit, we can fallback to      */
    /* using v itself as a bound; since v=5*n^2, sqrt(v)=n*sqrt(5) <= 3*n,   */
    /* and 3*n <= v for n >= 1, thus v also works as a bound without       */
    /* overflow issues for seq_t.                                            */
    seq_t high = v;            /* safe upper bound for seq_t that does not overflow */
    seq_t ans  = 0;

    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t sq  = mid * mid;
        if (sq == v) {
            return mid;
        }
        if (sq < v) {
            low = mid + 1;
            ans = mid;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Compute a(n) for any n >= 1.                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; undefined for n=0 */
    seq_t nn   = (seq_t)n;
    seq_t prod = nn * nn;               /* n^2       */
    seq_t v    = (seq_t)5 * prod;        /* 5*n^2     */
    seq_t t    = isqrt_seq(v);           /* floor(n*sqrt(5)) */
    seq_t ans  = v - t * t;              /* a(n)      */
    return ans;
}
