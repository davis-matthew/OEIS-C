/*
 * A184533  a(n) = floor(1/{(2+n^3)^(1/3)}), where {}=fractional part.
 * https://oeis.org/A184533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 - (1-(-1)^n))/4 for n>1, a(1)=2
 * Range:     u8 n <= 6, u16 n <= 208, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1) {
        return (seq_t)2;
    }

    /* For n >= 2 use the closed form */
    seq_t nn   = (seq_t)n;                  /* cast index to seq_t */
    seq_t tmp  = (n & 1) ? (seq_t)2 : (seq_t)0;  /* 1-(-1)^n → 2 if n odd, 0 if even */
    seq_t num  = (seq_t)6 * nn * nn - tmp;      /* numerator of the formula   */

    return num / (seq_t)4;                     /* integer division is exact */
}
