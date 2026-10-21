/*
 * A240115  Schoenheim lower bound L(n,4,2).
 * https://oeis.org/A240115
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n/4)*ceil((n-1)/3))
 * Range:     u8 n <= 28, u16 n <= 578, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the theorem does not provide a value; any return
       is acceptable, but we return 0. */
    if (n < 4) return 0;

    /* k = ceil((n-1)/3) = floor((n+1)/3) for n>=4 (integer arithmetic) */
    seq_t k = ((seq_t)n + 1) / 3;

    /* a(n) = ceil(n * k / 4) = (n*k + 3) / 4 */
    seq_t prod = (seq_t)n * k;
    return (prod + 3) / 4;
}
