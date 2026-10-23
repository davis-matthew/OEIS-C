/*
 * A136423  Floor((x^n - (1-x)^n)/2 +.5) where x = (sqrt(4)+1)/2 = 3/2.
 * https://oeis.org/A136423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(((3/2)^n - (-1/2)^n)/2 + .5)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1   (so a(1) is the first term listed) */
#include <stdint.h>

/* compute a(n) for any seq_t and idx_t */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* If seq_t is small (≤64‑bit), use 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        __int128 pow3 = 1;
        for (idx_t i = 0; i < n; ++i) {
            pow3 *= 3;
        }
        __int128 pow2 = (__int128)1 << n;              /* 2^n */
        __int128 num = pow3 + pow2 + ((n & 1) ? 1 : -1);/* numerator */
        return (seq_t)(num >> (n + 1));                /* divide by 2^(n+1) */
    }

    /* Otherwise seq_t is an arbitrary‑precision integer class. */
    /* compute 3^n by binary exponentiation */
    seq_t pow3 = 1;
    seq_t base = 3;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) {
            pow3 *= base;
        }
        base *= base;
        e >>= 1;
    }

    /* compute 2^n as an arbitrary‑precision integer */
    seq_t pow2 = 1;
    pow2 <<= n;                                            /* 2^n */

    /* numerator = 3^n + 2^n - (-1)^n */
    seq_t num = pow3 + pow2;
    if (n & 1) {
        num += 1;   /* odd n: +1 */
    } else {
        num -= 1;   /* even n: -1 */
    }

    /* a(n) = floor((num) / 2^(n+1)) */
    return num >> (n + 1);
}
