/*
 * A042965  Nonnegative integers not congruent to 2 mod 4.
 * https://oeis.org/A042965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor((4*n-3)/3)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS definition; for n==0 we return 0 as a safe fallback */
    if (n == 0) return 0;
    idx_t temp = 4 * n - 3;   /* compute numerator in 64‑bit to avoid overflow */
    return (seq_t)temp / 3;   /* integer division gives the floor */
}
