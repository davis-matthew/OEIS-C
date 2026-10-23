/*
 * A146529  a(0) = 6, a(n) = v(n) + v(n-1) - 2 where v(0)=4 and v(n) = 2^n +
 *          4.
 * https://oeis.org/A146529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=6, a(1)=8, for n≥2, a(n)=6+3*2^(n-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special first two terms */
    if (n == 0)
        return 6;
    if (n == 1)
        return 8;

    /* Compute 2^(n-1) by repeated multiplication to work with all
       integer types and arbitrary‑precision builds. */
    seq_t pow = 1;
    idx_t i = 0;
    idx_t limit = n - 1;
    for (; i < limit; ++i)
        pow *= 2;

    /* Final value: 6 + 3 * 2^(n-1) */
    return 6 + ( (seq_t)3 * pow );
}
