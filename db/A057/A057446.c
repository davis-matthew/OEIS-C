/*
 * A057446  To get next term, multiply by 13, add 1 and discard any prime
 *          factors < 13.
 * https://oeis.org/A057446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: a(n) = {73, 19, 31, 101}[n mod 4] */
seq_t a(idx_t n)
{
    static const seq_t cycle[4] = {73, 19, 31, 101};
    return cycle[(int)(n % 4)];
}
