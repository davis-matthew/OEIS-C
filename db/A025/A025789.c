/*
 * A025789  Expansion of 1/((1-x)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A025789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 18*n + 9*(2+(n mod 8))*(8-(n mod 8)) - 8*(n mod
 *            9)*(9-(n mod 9)))/144
 * Range:     u8 n <= 7, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t mod8 = ns % (seq_t)8;
    seq_t mod9 = ns % (seq_t)9;

    seq_t term = ns * ns;                     /* n^2 */
    term += (seq_t)18 * ns;                   /* + 18*n */
    term += (seq_t)9 * ((seq_t)2 + mod8) * ((seq_t)8 - mod8);  /* + 9*(2+mod8)*(8-mod8) */
    term -= (seq_t)8 * mod9 * ((seq_t)9 - mod9);               /* - 8*mod9*(9-mod9) */

    return term / (seq_t)144;
}
