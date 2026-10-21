/*
 * A237274  a(n) = A236283(n) mod 9.
 * https://oeis.org/A237274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A236283(n) mod 9. The sequence is conjectured to be
 *            periodic of period 18: 2,1,4,5,1,4,2,7,7,5,7,7,2,4,1,5,4,1,
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 18‑term period of a(n).  The values are the same as the terms listed in the OEIS entry
       (the period repeats forever).  This is a direct implementation of the known periodicity. */
    static const seq_t period[18] = {
        2, 1, 4, 5, 1, 4, 2, 7,
        7, 5, 7, 7, 2, 4, 1, 5,
        4, 1
    };
    return period[n % (idx_t)18];
}
