/*
 * A014766  Numbers k such that the 3k shuffle group does not accomplish a
 *          perfect shuffle.
 * https://oeis.org/A014766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, nice
 * Formula:   a(n) = nth smallest integer k such that * k = 3^(m+1) for some m
 *            ≥ 1 or k = 12*m for m ≥ 1
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */

    /* sequences: powers of 3 starting at 3^2 = 9,
     * multiples of 12 starting at 12*1 = 12.  */
    idx_t exp = 2;                 /* current exponent for 3^exp */
    seq_t pow3 = 9;                /* 3^exp */

    idx_t mult_idx = 1;            /* current multiplier for 12*m */
    seq_t mult = 12;               /* 12*mult_idx */

    seq_t result = 0;

    for (idx_t i = 1; i <= n; ++i) {
        if (mult <= pow3) {
            result = mult;
            ++mult_idx;
            mult = (seq_t)12 * (seq_t)mult_idx;
        } else {
            result = pow3;
            ++exp;
            pow3 = pow3 * 3;
        }
    }

    return result;
}
