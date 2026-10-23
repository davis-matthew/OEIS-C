/*
 * A021296  Decimal expansion of 1/292.
 * https://oeis.org/A021296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A021296 – decimal expansion of 1/292
   a(0) = 0, a(1) = 0;
   for n ≥ 2  : a(n) = {3,4,2,4,6,5,7,5}[(n-2) mod 8]          */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)0;

    static const seq_t pattern[8] = {
        (seq_t)3, (seq_t)4, (seq_t)2,
        (seq_t)4, (seq_t)6, (seq_t)5,
        (seq_t)7, (seq_t)5
    };
    return pattern[(n - 2) & 7];
}
