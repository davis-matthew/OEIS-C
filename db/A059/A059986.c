/*
 * A059986  Number of rods required to make a 3-D cube of side length n.
 * https://oeis.org/A059986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * n * (n + 1)^2
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type early to avoid accidental
       narrowing before multiplication.  All arithmetic is performed
       with seq_t, which may be an unsigned 8/16/32/64-bit integer or
       an arbitrary-precision type.  */
    seq_t nn = (seq_t)n;
    seq_t inc = nn + 1;
    /* a(n) = 3 * n * (n + 1)^2.  The intermediate results stay
       within the capacity of seq_t for the supported input ranges. */
    return 3 * nn * inc * inc;
}
