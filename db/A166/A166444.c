/*
 * A166444  a(0) = 0, a(1) = 1 and for n > 1, a(n) = sum of all previous
 *          terms.
 * https://oeis.org/A166444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, and for n > 1, a(n) = 2^(n-2), equivalently
 *            the sum of all previous terms. This implements a(0) = 0, a(1) =
 *            1, a(n) = a(0)+…+a(n‑1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    if (n == 1u)
        return (seq_t)1;
    /* For n >= 2: a(n) = 2^(n-2) = 1 << (n-2) */
    return (seq_t)1 << (n - 2u);
}
