/*
 * A085002  a(n) = floor(phi*n) - 2*floor(phi*n/2) where phi is the golden
 *          ratio.
 * https://oeis.org/A085002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(phi*n) - 2*floor(phi*n/2) (phi = (1+sqrt(5))/2)
 * Range:     u8 n <= 8898, u16 n <= 8898, u32 n <= 8898, u64 n <= 8898,
 *            u128 n <= 8898, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128, returns floor(sqrt(x)) */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64; /* sqrt(2^128-1) < 2^64 */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;   /* upper mid */
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main function for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard against 0 */
    unsigned __int128 nn   = n;
    unsigned __int128 nine = nn * nn;      /* n^2 */
    unsigned __int128 t    = nine * 5;      /* 5*n^2 */

    unsigned __int128 sqrt5n128 = isqrt128(t);            /* floor(sqrt(5*n^2)) == floor(n*sqrt(5)) */
    uint64_t sqrt5n = (uint64_t)sqrt5n128;               /* fits in 64 bits for n up to very large */

    /* Compute floor(phi*n) and floor(phi*n/2) using integer arithmetic */
    uint64_t sum            = n + sqrt5n;          /* (n + n*sqrt5) */
    uint64_t floor_phi_n    = sum / 2;             /* floor((n + sqrt5n)/2) == floor(phi*n) */
    uint64_t floor_phi_n_half = sum / 4;           /* floor((n + sqrt5n)/4) == floor(phi*n/2) */

    seq_t result = (seq_t)(floor_phi_n - 2 * floor_phi_n_half);
    return result;                                 /* result is 0 or 1 */
}
