/*
 * A064680  Halve every even number, double every odd number.
 * https://oeis.org/A064680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 for even n; a(n) = 2*n for odd n
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A064680: Halve every even number, double every odd number. */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;           /* convert index to sequence type */
    return (n & 1U) ? (n_seq << 1)    /* odd: double */
                    : (n_seq >> 1);   /* even: halve */
}
