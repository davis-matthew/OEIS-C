/*
 * A136958  Numbers k such that k and k^2 use only the digits 0, 4, 7 and 8.
 * https://oeis.org/A136958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136958: numbers k such that k and k² use only the digits 0, 4, 7 and 8. */
/* The terms (after the first zero) alternate between 88·10^m and 878·10^m (m≥0). */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t pow10_u(idx_t m)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < m; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)              /* a(1) = 0 */
        return (seq_t)0;

    idx_t m   = (n - 2) / 2;          /* exponent of 10 */
    bool   oop = ((n - 2) & 1);        /* 0 -> 88, 1 -> 878  */

    seq_t pow = pow10_u(m);            /* 10^m */

    if (oop)
        return pow * (seq_t)878;       /* odd (n-2)  -> 878·10^m */
    else
        return pow * (seq_t)88;        /* even (n-2) -> 88·10^m */
}
