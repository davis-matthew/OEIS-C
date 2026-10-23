/*
 * A168119  a(n) = n*(n^10+1)/2.
 * https://oeis.org/A168119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^10+1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 3390,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    seq_t pow = 1;
    for (idx_t i = 0; i < 10; ++i) {
        pow *= sn;
    }
    /* Defer the division by 2 until after *or* before multiplication
       to avoid overflow when n or n^10+1 is even. */
    if ((sn & 1) == 0) {
        return (sn / 2) * (pow + 1);
    } else {
        return sn * ((pow + 1) / 2);
    }
}
