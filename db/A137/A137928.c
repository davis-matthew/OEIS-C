/*
 * A137928  The even principal diagonal of a 2n X 2n square spiral.
 * https://oeis.org/A137928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + (n mod 2)
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^2 in a type that can hold it without overflow for n up to 10^4
       (idx_t is a 64‑bit unsigned integer).  The result is then cast back to seq_t. */
    idx_t sq = n * n;
    sq += (n & 1);      /* n mod 2 */
    return (seq_t)sq;
}
