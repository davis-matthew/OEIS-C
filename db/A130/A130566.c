/*
 * A130566  Pyramidal 47-gonal numbers.
 * https://oeis.org/A130566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n + 1)*(n + 2)*(n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 829, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;          /* cast index to seq_t first */
    seq_t p1  = 15 * sn + 1;       /* 15*n + 1 */
    seq_t p2  = sn + 2;            /* n + 2   */
    seq_t p3  = sn + 1;            /* n + 1   */

    /* compute (p1 * p2) / 2 first to keep intermediate size smaller */
    seq_t prod = (p1 * p2) / 2;
    return prod * p3;
}
