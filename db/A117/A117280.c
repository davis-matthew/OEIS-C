/*
 * A117280  a(n) = n^r(n), where r(n) is the digital root of n.
 * https://oeis.org/A117280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n^r(n), r(n) = digital root of n (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 → a(0) = 1 (as defined in OEIS) */
    if (n == 0)
        return (seq_t)1;

    /* Digital root for n > 0 : ((n-1) mod 9) + 1     */
    idx_t dr = ((n - 1) % 9) + 1;

    seq_t base = (seq_t)n;
    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < dr; ++i)
        result *= base;

    return result;
}
