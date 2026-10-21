/*
 * A060787  a(n) = 18*(n - 2)*(2*n - 5).
 * https://oeis.org/A060787
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*(n-2)*(2*n-5)
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(2) is 0; for n>=3 the formula is positive */
    if (n <= 2)
        return 0;

    seq_t n_seq = (seq_t)n;        /* convert index to seq_t */
    seq_t x = n_seq - (seq_t)2;    /* (n-2) */
    seq_t y = (seq_t)2 * n_seq - (seq_t)5; /* (2*n-5) */
    return (seq_t)18 * x * y;      /* 18*(n-2)*(2*n-5) */
}
