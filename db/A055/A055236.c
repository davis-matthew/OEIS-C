/*
 * A055236  Sums of two powers of 4.
 * https://oeis.org/A055236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 4^A002262(n) + 4^A003056(n)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 135, u64 n <= 527,
 *            u128 n <= 2079, big any n
 * Helpers:   isqrt128, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4.h"
#include "../utils/isqrt128.h"

/*   A002262(n) = floor((sqrt(8*n+1)-1)/2)   */
static idx_t triangular_inverse(idx_t n)
{
    /* Compute floor((sqrt(8*n+1)-1)/2) without floating point */
    unsigned __int128 val = (unsigned __int128)8 * (unsigned __int128)n + 1;
    uint64_t sqrt_val = isqrt128(val);      /* floor(sqrt(8*n+1)) */
    return (idx_t)((sqrt_val - 1) >> 1);    /* divide by 2 */
}

seq_t a(idx_t n)
{
    idx_t t  = triangular_inverse(n);                      /* A002262(n) */
    idx_t tri = ( (unsigned __int128)t * (t + 1) ) / 2;   /* triangular number T_t   */
    idx_t s  = n - tri;                                    /* A003056(n)              */

    seq_t pow_t = pow4(t);   /* 4^t */
    seq_t pow_s = pow4(s);   /* 4^s */

    return pow_t + pow_s;
}
