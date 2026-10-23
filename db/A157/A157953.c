/*
 * A157953  a(n) = 81n^2 - n.
 * https://oeis.org/A157953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n^2 - n
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  a(1) = 80, a(2) = 322, …  */
    if (n < 1) return 0;            /* behaviour below the offset is irrelevant */

    seq_t nn   = (seq_t)n;          /* convert index to the correct type */
    seq_t prod = (seq_t)81 * nn;    /* 81*n  (safe in seq_t type) */
    return nn * (prod - (seq_t)1);  /* n*(81*n - 1) = 81*n^2 - n */
}
