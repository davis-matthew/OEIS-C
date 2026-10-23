/*
 * A025764  Expansion of 1/((1-x)(1-x^2)(1-x^7)).
 * https://oeis.org/A025764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n+3)*(n+7)/28)
 * Range:     u8 n <= 10, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of 1/((1-x)(1-x^2)(1-x^7)). */
seq_t a(idx_t n)
{
    /* Compute round((n+3)*(n+7)/28).  For positive arguments,
       round(x) = floor(x + 0.5), hence
       round((n+3)*(n+7)/28) = ((n+3)*(n+7) + 14) / 28. */
    seq_t num = ((seq_t)(n) + 3) * ((seq_t)(n) + 7);
    num += 14;
    return num / 28;
}
