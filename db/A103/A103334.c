/*
 * A103334  Number of closed walks of length 2n at any of the nodes of degree
 *          1 on the graph of the (7,4) Hamming code.
 * https://oeis.org/A103334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=4; for n >= 3 a(n) = 10 * a(n-1) - 16 *
 *            a(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence is derived from the generating
   function (1-9x+10x^2)/(1-10x+16x^2) of sequence A103334. */

seq_t a(idx_t n)
{
    if (n == 0)   { return (seq_t)1; }
    if (n == 1)   { return (seq_t)1; }
    if (n == 2)   { return (seq_t)4; }

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)4;   /* a(2) */
    seq_t current;

    for (idx_t k = 3; k <= n; ++k) {
        current = (seq_t)10 * prev1 - (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;   /* a(n) */
}
