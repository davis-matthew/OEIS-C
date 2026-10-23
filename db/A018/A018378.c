/*
 * A018378  Divisors of 292.
 * https://oeis.org/A018378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 0 for n > 6 2^(n-1) for n = 1,2,3 73 * 2^(n-4) for n =
 *            4,5,6 This generates the divisors of 292 in ascending order
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, so n=0 is undefined */
    if (n <= 3u)                   /* 1, 2, 4 */
        return ((seq_t)1) << (((int)n) - 1);
    if (n <= 6u)                   /* 73, 146, 292 */
        return ((seq_t)73) << (((int)n) - 4);
    return (seq_t)0;               /* no further divisors */
}
