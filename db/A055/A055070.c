/*
 * A055070  Third column of triangle A055864.
 * https://oeis.org/A055070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n + 1)^(n − 3), with a(1) = a(2) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a single power of a seq_t base to an idx_t exponent. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;   /* multiply when corresponding bit is set */
        base *= base;                  /* square the base */
        exp >>= 1;                     /* move to next bit */
    }
    return result;
}

/* Required external function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    seq_t nseq   = (seq_t)n;
    seq_t nsq    = nseq * nseq;                     /* n^2 */
    seq_t base   = (seq_t)(n + 1);                  /* n + 1 */
    seq_t pow    = pow_seq_t(base, n - 3);          /* (n + 1)^(n−3) */

    return nsq * pow;                              /* n^2 * (n + 1)^(n−3) */
}
