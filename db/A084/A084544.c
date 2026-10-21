/*
 * A084544  Alternate number system in base 4.
 * https://oeis.org/A084544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{j=0..m-1} (1 + floor((3*n+1-4^m)/(3*4^j)) mod 4) *
 *            10^j, where m = floor(log_4(3*n+1))
 * Range:     u8 n <= 52, u16 n <= 1364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A084544 (alternative base-4 number system). */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 3*n+1 in seq_t to avoid overflow of idx_t */
    seq_t prod3n1 = (seq_t)3 * n + 1;

    /* Determine m and 4^m (pow4m) such that 4^m <= 3*n+1 < 4^(m+1) */
    idx_t      m      = 0;
    seq_t      pow4m  = 1;
    const seq_t FOUR   = (seq_t)4;
    while ( FOUR * pow4m <= prod3n1 )
    {
        pow4m *= FOUR;
        m++;
    }

    /* Remainder after subtracting 4^m */
    seq_t r = prod3n1 - pow4m;

    /* Sum the contribution of each digit */
    seq_t sum     = 0;
    seq_t pow4j   = 1;          /* 4^j */
    const seq_t THREE = (seq_t)3;

    for ( idx_t j = 0; j < m; j++ )
    {
        seq_t denom = THREE * pow4j;          /* 3 * 4^j */
        seq_t div   = r / denom;             /* floor((3*n+1-4^m)/(3*4^j)) */
        seq_t digit = 1 + (div % 4);          /* digit between 1 and 4 */
        sum += digit * pow10( j );           /* 10^j * digit */

        pow4j *= FOUR;                       /* next power of 4 */
    }

    return sum;
}
