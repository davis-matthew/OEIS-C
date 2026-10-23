/*
 * A177189  Partial sums of round(n^2/16).
 * https://oeis.org/A177189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3)*(2*n^2-3*n+13)/96)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 885, u64 n <= 885,
 *            u128 n <= 885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid any overflow when
       computing with larger types. */
    seq_t x = (seq_t)n;

    /* Compute 2*n^2 - 3*n + 13 */
    seq_t t1 = 2 * x * x;   /* 2*n^2 */
    t1 -= 3 * x;            /* 2*n^2 - 3*n */
    t1 += 13;               /* 2*n^2 - 3*n + 13 */

    /* Multiply by (n + 3) and perform integer division by 96.
       Since all values are non‑negative, integer division is the
       same as floor division. */
    seq_t prod = t1 * (x + 3);
    return prod / 96;
}
