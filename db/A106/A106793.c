/*
 * A106793  Number of words (over an alphabet of size 26) of length n with all
 *          different letters.
 * https://oeis.org/A106793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = 26! / (26 - n)!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mnemonic: number of length‑n words over a 26‑letter alphabet with all letters distinct. */

seq_t a(idx_t n)
{
    /* For n > 26 the number of distinct letters is insufficient – return 0. */
    if (n > (idx_t)26)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i)
        result *= (seq_t)(26 - i);

    return result;
}
