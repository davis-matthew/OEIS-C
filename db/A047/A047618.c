/*
 * A047618  Numbers that are congruent to {0, 2, 5} mod 8.
 * https://oeis.org/A047618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(8*(n-1)/3)
 * Range:     u8 n <= 96, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and shift to 0‑based */
    seq_t s = (seq_t)(n - 1);
    /* Compute floor(s/3) and remainder */
    seq_t t = s / 3;
    seq_t r = s % 3;
    /* 8 * r divided by 3 yields 0, 2, or 5 for r = 0,1,2 respectively */
    seq_t add = (8 * r) / 3;
    /* Result */
    return 8 * t + add;
}
