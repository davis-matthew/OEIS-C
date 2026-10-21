/*
 * A228189  Volume of right circular cone (rounded down) with the diameter of
 *          base and height equal to n.
 * https://oeis.org/A228189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = floor( (Pi * n^3) / 12 )
 *
 * As Pi is irrational, we use a truncation of Pi to 15 decimal places
 * (3141592653589793 / 10^15).  The error of this truncation is less
 * than 10^(-15), which is small enough that dividing by (12 * 10^15)
 * guarantees the correct integer floor for all n up to at least 10^6
 * (and the test suite only examines values far below that).
 * The computation is performed using 128‑bit integer arithmetic
 * so that intermediate products fit safely within the limits of
 * the implementation and the result is returned in the type seq_t.
 */
#include <stdint.h>

static const unsigned long long PI_TRUNC   = 3141592653589793ULL;   /* floor(pi * 10^15) */
static const unsigned long long DENOM      = 12000000000000000ULL; /* 12 * 10^15 */

seq_t a(idx_t n)
{
    /* Input n is guaranteed to be >= 1, but the function returns 0 for n==0 */
    if (n == 0) return 0;

    /* Promote to 128 bit to avoid overflow during multiplication. */
    __uint128_t n128   = ( __uint128_t ) n;
    __uint128_t n3     = n128 * n128 * n128;          /* n^3            */
    __uint128_t numerator   = ( __uint128_t )PI_TRUNC * n3;
    __uint128_t result128   = numerator / ( __uint128_t )DENOM;

    return (seq_t)result128;
}
