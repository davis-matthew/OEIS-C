/*
 * A096431  Denominator of (9*(n^4 - 2*n^3 + 2*n^2 - n) + 2)/(2*(2*n - 1)).
 * https://oeis.org/A096431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of (9*(n^4 - 2*n^3 + 2*n^2 - n) + 2)/(2*(2*n
 *            - 1))
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 1107, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t (supports unsigned 64/128-bit) */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* use 128‑bit intermediate to avoid overflow */
    __int128 nn = n;
    __int128 num = 9*nn*nn*nn*nn - 18*nn*nn*nn + 18*nn*nn - 9*nn + 2;
    __int128 den = 4*nn - 2;          /* 2*(2*n - 1) */

    seq_t num_seq = (seq_t)num;
    seq_t den_seq = (seq_t)den;
    seq_t g = gcd_seq_t(num_seq, den_seq);

    return den_seq / g;               /* simplified denominator */
}
