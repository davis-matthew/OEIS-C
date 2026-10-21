/*
 * A068380  Engel expansion of sinh(1/3).
 * https://oeis.org/A068380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*(n*(2*n-3)+1) for n > 1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    /* Convert index to seq_t once to avoid repeated casts */
    seq_t n_st = (seq_t)n;
    /* Compute n*(2*n-3)+1 */
    seq_t term = n_st * (n_st * (seq_t)2 - (seq_t)3) + (seq_t)1;
    /* Multiply by 18 */
    return (seq_t)18 * term;
}
