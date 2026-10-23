/*
 * A117230  Start with 1 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A117230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = ((n-2) mod 9) + 2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    idx_t r = (n - 2) % 9;   /* compute the cycle position */
    return (seq_t)(r + 2);
}
