/*
 * A118517  Define sequence S_m by: initial term = m, reverse digits and add 3
 *          to get next term. Entry shows S_1. This reaches a cycle of length
 *          3 in 1 step.
 * https://oeis.org/A118517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1,
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         for n >= 2, a(n) = 4 + 3 * ((n-2) mod 3) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined case, return 0 safely */
    if (n == 1)
        return 1;                 /* a(1) = 1 */
    seq_t mod = (seq_t)((n - 2) % 3);   /* (n-2) modulo 3, fits in seq_t */
    return 4 + 3 * mod;          /* cycle 4,7,10 */
}
