/*
 * A041246  Numerators of continued fraction convergents to sqrt(135).
 * https://oeis.org/A041246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for numerators of convergents to sqrt(135):
   coeff(0)=11, coeff(n)=22 if n%8==0 else 1 for n>0.
   a(0)=11, a(1)=12.
   a(n)=coeff(n)*a(n-1)+a(n-2) for n>=2. */
seq_t a(idx_t n)
{
    seq_t p_minus2 = 0;   /* a(-2) */
    seq_t p_minus1 = 1;   /* a(-1) */
    seq_t p_cur    = 0;   /* current a(n) */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = 11;
        } else if (i % 8 == 0) {
            coeff = 22;
        } else {
            coeff = 1;
        }
        p_cur = coeff * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_cur;
    }
    return p_cur;
}
