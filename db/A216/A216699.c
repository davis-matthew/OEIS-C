/*
 * A216699  Digital root of cubes of Fibonacci numbers.
 * https://oeis.org/A216699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of (Fibonacci(n))³ Equivalent to a(n) = 0 (n
 *            = 0) or a(n) = ((Fibonacci(n)³ – 1) mod 9) + 1 for n > 0. The
 *            sequence is periodic with period 8 for n > 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* mapping for n % 8: 0 → 9, 1 → 1, 2 → 1, 3 → 8, 4 → 9,
       5 → 8, 6 → 8, 7 → 1 (for n > 0).  n = 0 is a special case. */
    static const seq_t period[8] = {9, 1, 1, 8, 9, 8, 8, 1};

    if (n == 0)
        return 0;
    return period[n % 8];
}
