/*
 * A180103  Floor( 100*(n-1)/n ).
 * https://oeis.org/A180103
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(100*(n-1)/n)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                /* undefined for n < 2, offset is 2 */

    /* For n > 100, (n-1)/n < 1, so the floor is 99. */
    if (n > 100)
        return 99;

    /* For n <= 100 the intermediate product fits in seq_t
       (100 * 99 = 9900), avoiding overflow. */
    return ((seq_t)100 * (n - 1)) / n;
}
