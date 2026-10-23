/*
 * A199492  a(n) = 11*7^n + 1.
 * https://oeis.org/A199492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 11*7^n + 1
 * Recurrence: a(0) = 12
 *              a(n) = 7*a(n-1) - 6
 */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)12;          /* a(0) */
    for (idx_t i = 0; i < n; ++i)  /* compute a(1) … a(n) */
        val = val * (seq_t)7 - (seq_t)6;
    return val;
}
