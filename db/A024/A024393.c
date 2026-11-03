/*
 * A024393  4th elementary symmetric function of the first n+3 positive
 *          integers congruent to 2 mod 3.
 * https://oeis.org/A024393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(405*n^4+4590*n^3+18495*n^2+30534*n+16376)/1920
 * Range:     u8 none, u16 n <= 2, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t to_seq128(__uint128_t v)
{
    /* Convert a 128‑bit integer to seq_t.
     * If seq_t fits in 64 bits we truncate the lower 64 bits.
     * Otherwise we rebuild the value using a 64‐bit shift and addition. */
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((unsigned long long)v);
    } else {
        seq_t low  = (seq_t)((unsigned long long)v);
        seq_t high = (seq_t)((unsigned long long)(v >> 64));
        return (high << 64) + low;
    }
}

seq_t a(idx_t n)
{
    __uint128_t nn  = n;
    __uint128_t n2  = nn * nn;
    __uint128_t n3  = n2 * nn;
    __uint128_t n4  = n3 * nn;

    /* Polynomial part */
    __uint128_t poly = (405u * n4)
                     + (4590u * n3)
                     + (18495u * n2)
                     + (30534u * nn)
                     + 16376u;

    /* Product of the four consecutive numbers */
    __uint128_t t = nn * (nn + 1) * (nn + 2) * (nn + 3);

    /* Numerator and division by 1920 */
    __uint128_t num = t * poly;
    __uint128_t res = num / 1920u;

    return to_seq128(res);
}
