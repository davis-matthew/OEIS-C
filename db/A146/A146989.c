/*
 * A146989  Periodic sequence with period {15,2,1,13,1}.
 * https://oeis.org/A146989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = period[n mod 5], period = {15, 2, 1, 13, 1}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period array for the 5‑term repeating cycle */
    static const seq_t period[5] = { (seq_t)15, (seq_t)2, (seq_t)1, (seq_t)13, (seq_t)1 };
    return period[n % 5];
}
