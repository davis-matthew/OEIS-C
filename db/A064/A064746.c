/*
 * A064746  a(n) = n*8^n + 1.
 * https://oeis.org/A064746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*8^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence 8^0 = 1, 8^k = 8*8^(k-1) */
seq_t a(idx_t n)
{
    seq_t pow = (seq_t)1;          /* 8^0 */
    for (idx_t i = 0; i < n; ++i)  /* compute 8^n iteratively */
        pow *= (seq_t)8;
    return pow * (seq_t)n + (seq_t)1;
}
