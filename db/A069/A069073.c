/*
 * A069073  a(n) = n*(4n^2 - 1)^2.
 * https://oeis.org/A069073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(4n^2 - 1)^2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A069073. */
seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* cast index to seq_t */
    seq_t tmp = 4 * ns * ns;     /* 4n^2 */
    seq_t inner = tmp - 1;       /* 4n^2 - 1 */
    return ns * inner * inner;   /* n * (4n^2 - 1)^2 */
}
