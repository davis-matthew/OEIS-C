/*
 * A062717  Numbers m such that 6*m+1 is a perfect square.
 * https://oeis.org/A062717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n*(n-1) + (2*n-1)*(-1)^n + 1)/4
 * Range:     u8 n <= 7, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of numbers m such that 6*m+1 is a perfect square. */

seq_t a(idx_t n)
{
    /* n is 1‑based index, always >= 1 in the context of this problem */
    seq_t n_seq   = (seq_t)n;          /* convert index to the sequence type */
    seq_t two_n   = (seq_t)(2 * n_seq);
    seq_t term;

    if ((n & 1) == 0)         /* n even: (-1)^n = +1  */
    {
        term = (seq_t)6 * n_seq * (n_seq - 1) + two_n - 1 + 1;
    }
    else                      /* n odd:  (-1)^n = -1  */
    {
        term = (seq_t)6 * n_seq * (n_seq - 1) - (two_n - 1) + 1;
    }

    return term / 4;
}
