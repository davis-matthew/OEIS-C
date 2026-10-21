/*
 * A061085  a(n) = A019550(n) / 3.
 * https://oeis.org/A061085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = one-third of the number obtained by concatenating n with
 *            2n
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t */
    seq_t nn = (seq_t)n;
    /* 2n */
    seq_t two_n = nn + nn;

    /* Count decimal digits of 2n */
    idx_t k = 0;
    seq_t tmp = two_n;
    do {
        tmp /= 10;
        k++;
    } while (tmp);

    /* Compute 10^k */
    seq_t pow10k = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow10k *= 10;

    /* Concatenate n with 2n: n * 10^k + 2n */
    seq_t concat = nn * pow10k + two_n;

    /* Return one‑third of the concatenated number */
    return concat / 3;
}
