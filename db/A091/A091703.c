/*
 * A091703  Count, setting 5n to zero.
 * https://oeis.org/A091703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = n^5 mod (5*n) * (equivalently, a(n) = n for n mod 5 ≠ 0,
 *            otherwise a(n) = 0)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is a multiple of 5, the result is 0; otherwise the result is n.
       This follows directly from the modular reduction n^5 mod (5*n). */
    if (n % 5 == 0)
        return (seq_t)0;
    return (seq_t)n;
}
