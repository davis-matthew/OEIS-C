/*
 * A210978  A186029 and positive terms of A001106 interleaved.
 * https://oeis.org/A210978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(-1+(-1)^n)+2*(5+(-1)^n)*n+14*n^2)/16 Recurrence:
 *            a(n)=a(n-1)+2*a(n-2)-2*a(n-3)-a(n-4)+a(n-5) Initial values:
 *            a(0)=0, a(1)=1, a(2)=5, a(3)=9, a(4)=17 Uses a 128‑bit signed
 *            intermediate on implementations that support __int128, otherwise
 *            the expression fits into signed 64‑bit for all pra...
 * Range:     u8 n <= 16, u16 n <= 273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* perform calculation in signed 128‑bit integer to avoid overflow */
    __int128 sn = (__int128)n;
    __int128 parity = (n & 1U) ? -1 : 1;          /* (-1)^n */
    __int128 val = (3 * (-1 + parity)          /* 3*(-1+(-1)^n)   */
                    + 2 * (5 + parity) * sn   /* 2*(5+(-1)^n)*n  */
                    + 14 * sn * sn) / 16;      /* +14*n^2         */
    return (seq_t)val;
}
