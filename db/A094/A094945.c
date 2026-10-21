/*
 * A094945  Initial n terms of A023532(n) taken as digits.
 * https://oeis.org/A094945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Σ_{k=0..n} A023532(k) * 10^(n-k) where A023532(k) = 0 if
 *            k = t(t+3)/2 for some integer t ≥ 0, otherwise A023532(k) = 1.
 *            Hence a(n) is the decimal number formed by the digits
 *            A023532(0), A023532(1), …, A023532(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i;
    seq_t res = 0;

    /* D = 9 + 8*i.  For k = t(t+3)/2, 9 + 8*k = (2t+3)^2. */
    uint64_t D       = 9;   /* 9 + 8*0  */
    uint64_t sqrt_t  = 3;   /* floor(sqrt(9)) = 3  */

    for (i = 0; i <= n; ++i) {
        /* Test whether i is of the form t(t+3)/2  */
        bool zero = (
            ( (__uint128_t)sqrt_t * sqrt_t == D ) &&
            ( (sqrt_t >= 3) && ((sqrt_t - 3) & 1) == 0 )
        );

        /* Append the digit 0 or 1 to the decimal number */
        res = res * 10 + (zero ? 0 : 1);

        /* Advance to next index: D = 9 + 8*(i+1)  */
        D += 8;

        /* Update sqrt_t to be floor(sqrt(D)) */
        while ( (__uint128_t)(sqrt_t + 1) * (sqrt_t + 1) <= D )
            ++sqrt_t;
        while ( (__uint128_t)sqrt_t * sqrt_t > D )
            --sqrt_t;
    }

    return res;
}
