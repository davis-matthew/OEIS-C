/*
 * A204769  a(n) = 151*(n-1) - a(n-1) with n>1, a(1)=46.
 * https://oeis.org/A204769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-151 - 33*(-1)^n + 302*n) / 4 for n >= 1, with a(1) = 46
 * Range:     u8 n <= 3, u16 n <= 868, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute (-1)^n: 1 if n even, -1 if n odd */
    int8_t s = (n & 1ULL) ? -1 : 1;

    /* use 128‑bit signed arithmetic for intermediate */
    __int128 val = -151 - 33 * s + (__int128)302 * n;
    val /= 4;            /* always divisible by 4 */

    return (seq_t)val;   /* cast to unsigned seq_t */
}
