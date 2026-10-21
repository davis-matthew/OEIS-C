/*
 * A054966  Numbers that are congruent to {0, 1, 8} mod 9.
 * https://oeis.org/A054966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*⌊(n-1)/3⌋ + {0,1,8}[ (n-1) mod 3 ] (for n ≥ 1)
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against zero, though the function is defined for n ≥ 1. */
    if (n == 0) return 0;

    idx_t m = n - 1;
    idx_t q = m / 3;       /* floor((n‑1)/3) */
    idx_t r = m % 3;       /* (n‑1) mod 3 */

    seq_t base = 9 * ((seq_t)q);  /* mult by constant 9, allowed */

    if (r == 0) return base;     /* offset 0 */
    if (r == 1) return base + 1; /* offset 1 */
    return base + 8;             /* offset 8 when r == 2 */
}
