/*
 * A131079  Periodic sequence (2, 2, 2, 1, 0, 0, 0, 1).
 * https://oeis.org/A131079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-8); periodic sequence (2, 2, 2, 1, 0, 0, 0, 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence repeats every 8 terms.  We map (n-1) modulo 8
       to the corresponding value in the period. */
    static const seq_t period[8] = {2, 2, 2, 1, 0, 0, 0, 1};
    idx_t idx = (n - 1) & 7U;          /* (n-1) % 8  using bitmask */
    return period[idx];
}
