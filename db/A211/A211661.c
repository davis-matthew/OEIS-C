/*
 * A211661  Number of iterations log_3(log_3(log_3(...(n)...))) such that the
 *          result is < 1.
 * https://oeis.org/A211661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if n<3, 2 if 3≤n<27, 3 if 27≤n<3^27, 4 otherwise
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts the number of times log base 3 must be applied to n
   to obtain a value smaller than 1. */
seq_t a(idx_t n)
{
    if (n < 3U)
        return (seq_t)1;
    else if (n < 27U)
        return (seq_t)2;
    else if (n < 7625597484987ULL) /* 3^27 */
        return (seq_t)3;
    else
        return (seq_t)4;
}
