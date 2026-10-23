/*
 * A131969  First differences of A020806.
 * https://oeis.org/A131969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3 when n ≡ 0 (mod 3) * a(n) = -2 * (-1)^(n/3) when n ≡ 1
 *            (mod 3) * a(n) = 6 * (-1)^(n/3) when n ≡ 2 (mod 3)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 6: 3, -2, 6, -3, 2, -6. */
seq_t a(idx_t n)
{
    /* k = n/3 gives the exponent of (-1) in the closed form. */
    idx_t k = n / 3;
    /* remainder modulo 3 selects the coefficient (3, -2, or 6). */
    int rem = (int)(n % 3);
    /* Sign factor (-1)^k: 1 if k even, -1 if k odd. */
    seq_t sign = ((k & 1) == 0) ? 1 : -1;

    switch (rem) {
        case 0: return 3  * sign;
        case 1: return -2 * sign;
        case 2: return 6  * sign;
        default: /* unreachable */ return 0;
    }
}
