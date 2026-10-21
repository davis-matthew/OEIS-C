/*
 * A060887  a(n) = Sum_{j=0..12} n^j.
 * https://oeis.org/A060887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..12} n^j
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any nonnegative n.  The function works for all
   integer types provided for seq_t and idx_t, making use only of
   integer arithmetic and the standard C operators. */
seq_t a(idx_t n)
{
    seq_t total = (seq_t)1;     /* term for j = 0 (n^0 = 1) */
    seq_t power = (seq_t)1;     /* current n^j */
    for (idx_t j = 1; j <= 12; ++j)
    {
        power *= (seq_t)n;      /* compute n^j */
        total += power;        /* accumulate */
    }
    return total;
}
