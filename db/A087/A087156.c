/*
 * A087156  Nonnegative numbers excluding 1.
 * https://oeis.org/A087156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; a(n) = n for n > 1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence consists of all non‑negative integers except 1.
       It has the closed form: a(1)=0, a(n)=n for n>1. */
    return (n > 1) ? (seq_t)n : (seq_t)0;
}
