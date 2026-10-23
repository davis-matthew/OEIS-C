/*
 * A025793  Expansion of 1/((1-x)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A025793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 22*n + 11*(2 + (n mod 10))*(10 - (n mod 10))
 *            -10*(n mod 11)*(11 - (n mod 11)))/220
 * Range:     u8 n <= 9, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for safe arithmetic */
    seq_t nn   = (seq_t)n;
    seq_t mod10 = (seq_t)(n % 10);
    seq_t mod11 = (seq_t)(n % 11);

    seq_t numerator =
        nn * nn                /* n^2 */
        + (seq_t)22 * nn      /* + 22*n */
        + (seq_t)11 * ((seq_t)2 + mod10) * ((seq_t)10 - mod10)
        - (seq_t)10 * mod11 * ((seq_t)11 - mod11);

    return numerator / (seq_t)220;
}
