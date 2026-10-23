/*
 * A036087  Centered cube numbers: a(n) = (n+1)^9 + n^9.
 * https://oeis.org/A036087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^9 + n^9
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 10, u64 n <= 127,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow9(seq_t x)
{
    /* compute x^9 using repeated squaring, all in seq_t */
    seq_t x2 = x * x;          /* x^2 */
    seq_t x4 = x2 * x2;        /* x^4 */
    seq_t x8 = x4 * x4;        /* x^8 */
    return x8 * x;             /* x^9 */
}

seq_t a(idx_t n)
{
    seq_t nseq   = (seq_t)n;
    seq_t n1seq  = (seq_t)(n + 1);

    seq_t n9  = pow9(nseq);
    seq_t n19 = pow9(n1seq);

    return n9 + n19;
}
