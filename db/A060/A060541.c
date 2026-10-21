/*
 * A060541  a(n) = binomial(4*n,4).
 * https://oeis.org/A060541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n,4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 70, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No special handling for n < 1: behaviour below offset is irrelevant. */
    seq_t m      = (seq_t) n;     /* convert index to seq_t */
    seq_t four_n = (seq_t)4 * m;  /* 4*n */
    seq_t prod   = four_n * (four_n - (seq_t)1);
    prod         = prod * (four_n - (seq_t)2);
    prod         = prod * (four_n - (seq_t)3);
    const seq_t D = (seq_t)24;   /* 4! */
    return prod / D;             /* integer division; exact. */
}
