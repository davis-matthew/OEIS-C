/*
 * A168410  a(n) = 3 + 9*floor(n/2).
 * https://oeis.org/A168410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3 + 9*floor(n/2)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence undefined for n = 0; return 0 for safety */
    if (n == 0) return 0;
    seq_t half = (seq_t)(n / 2);   /* floor division */
    return 3 + 9 * half;
}
