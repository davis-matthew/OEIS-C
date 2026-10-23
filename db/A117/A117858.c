/*
 * A117858  Number of palindromes of length n (in base 6).
 * https://oeis.org/A117858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*6^floor((n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the out‑of‑range case (only for safety) */
    if (n == 0) return 0;

    /* exponent k = floor((n-1)/2) */
    idx_t k = (n - 1) / 2;

    /* compute 6^k */
    seq_t power = 1;
    for (idx_t i = 0; i < k; ++i)
        power *= 6;

    /* final value */
    return 5 * power;
}
