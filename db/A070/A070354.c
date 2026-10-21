/*
 * A070354  a(n) = 3^n mod 16.
 * https://oeis.org/A070354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 16 (cycle period 4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Modulo 16 of powers of 3 repeats every 4:
         n mod 4 : 0 1 2 3
         a(n)    : 1 3 9 11 */
    static const seq_t tbl[4] = {1, 3, 9, 11};
    return tbl[n & 3];            /* n is unsigned, so &3 == n%4 */
}
