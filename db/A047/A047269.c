/*
 * A047269  Numbers that are congruent to {0, 1, 2, 5} mod 6.
 * https://oeis.org/A047269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + 2*floor(n/4)
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the set of non‑negative integers congruent to 0, 1, 2 or 5 (mod 6)  */

seq_t a(idx_t n)
{
    if (n == 0)     /* undefined for n < 1 – return 0 to avoid underflow   */
        return 0;

    seq_t ns = (seq_t)n;                     /* cast index to sequence type */
    return ns - 1 + (ns / (seq_t)4) * (seq_t)2;
}
