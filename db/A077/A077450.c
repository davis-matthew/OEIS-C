/*
 * A077450  Continued fraction expansion of (29+sqrt(145))/12.
 * https://oeis.org/A077450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The CF expansion of (29 + sqrt(145))/12 is
   3; 2, 2, 1, 1, 1 repeating.  Thus
   a(0) = 3
   a(n) = {2, 2, 1, 1, 1}[ (n-1) mod 5 ],   for n ≥ 1. */
seq_t a(idx_t n)
{
    /* special first term */
    if (n == 0) return (seq_t)3;

    /* periodic part */
    static const seq_t period[5] = { (seq_t)2, (seq_t)2,
                                     (seq_t)1, (seq_t)1,
                                     (seq_t)1 };
    return period[(n - 1) % 5];
}
