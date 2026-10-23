/*
 * A134859  Wythoff AAA numbers.
 * https://oeis.org/A134859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(n*Phi^2) - n - 2, Phi = (1+sqrt5)/2
 * Range:     u8 n <= 61, u16 n <= 9879, u32 n <= 9879, u64 n <= 9879,
 *            u128 n <= 9879, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained from the upper Wythoff sequence:
   B(n) = floor(n*Phi^2) = A(n) + n,  where A is the lower Wythoff sequence.
   To avoid floating point arithmetic we use the identity
         floor(n*Phi^2) = floor( (3n + floor(n*sqrt5)) / 2 )
   and the fact that floor(n*sqrt5) = floor(sqrt(5*n*n)).  The latter can be
   computed with an integer square‑root routine that operates on 128–bit values
   so that it works for all n that fit into a uint64_t without overflow. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square–root on unsigned __int128, result fits in idx_t (≤64 bits) */
static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (idx_t)lo;
}

/* floor(n * sqrt(5)) for all n that fit into idx_t */
static idx_t floor_n_sqrt5(idx_t n)
{
    if (n == 0) return 0;
    unsigned __int128 prod = (unsigned __int128)5 * n * n;   /* 5·n²  */
    return isqrt_u128(prod);
}

seq_t a(idx_t n)
{
    /* lower Wythoff number A(n) = floor(n*phi) = (n + floor(n*sqrt5)) / 2 */
    idx_t sqrt5_n   = floor_n_sqrt5(n);
    idx_t A_n       = (n + sqrt5_n) >> 1;          /* integer division by 2 */

    /* upper Wythoff number B(n) = A(n) + n */
    idx_t B_n = A_n + n;

    /* AAA number a(n) = 2*B(n) - n - 2 */
    /* All intermediate results fit into seq_t if they fit in idx_t. */
    return (seq_t)( (seq_t)2 * (seq_t)B_n - (seq_t)n - (seq_t)2 );
}
