/*
 * A202199  Number of (n+2) X 7 binary arrays avoiding patterns 001 and 101 in
 *          rows and columns.
 * https://oeis.org/A202199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(n+7)*(n+6)*(n+5)*(n+4)*(n+3)*(n+2)^2/360, n>=1
 * Range:     u8 none, u16 n <= 4, u32 n <= 37, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 u128;
#else
    /* If the compiler does not support 128-bit types, fallback to a
       simple 64-bit implementation that works for small n.  This
       path is only used for builds where 64‑bit values are sufficient. */
    u128 prod = 1; /* will actually be a 64-bit integer */
#endif

    /* Compute the product in an intermediate 128‑bit type */
    u128 prod = 1;
    prod *= (u128)7;
    prod *= (u128)(n + 7);
    prod *= (u128)(n + 6);
    prod *= (u128)(n + 5);
    prod *= (u128)(n + 4);
    prod *= (u128)(n + 3);
    prod *= (u128)(n + 2);  /* first factor (n+2) */
    prod *= (u128)(n + 2);  /* second factor (n+2) */

    prod /= (u128)360;      /* divide by 360 */

    return (seq_t)prod;
}
