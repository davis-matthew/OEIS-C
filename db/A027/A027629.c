/*
 * A027629  Molien series for complex 8-dimensional group N_3 of order
 *          2^(2.3+2), a central extension of an extraspecial 2-group.
 * https://oeis.org/A027629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (630 + 2376*n + 2891*n^2 + 1673*n^3 + 980*n^4 + 644*n^5 +
 *            224*n^6 + 32*n^7) / 630
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 35, u64 n <= 864, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses Horner's method with 128‑bit intermediate arithmetic to avoid overflow for all supported seq_t sizes. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit signed integer for intermediate calculations.
       This type is available as a GCC/Clang extension and works on
       both C11 and C++17 compilers. */
    __int128_t x = (__int128_t)n;
    __int128_t num = 32;
    num = num * x + 224;
    num = num * x + 644;
    num = num * x + 980;
    num = num * x + 1673;
    num = num * x + 2891;
    num = num * x + 2376;
    num = num * x + 630;
    /* The division yields an integer result; cast back to seq_t. */
    return (seq_t)(num / 630);
}
