/*
 * A094283  Row sums of triangle A094280.
 * https://oeis.org/A094283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45 * 2^(n-4) for n >= 4 a(1) = 1, a(2) = 5, a(3) = 15
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)15;

    idx_t k = n - 4;                /* exponent of 2 */
    seq_t pow2 = (seq_t)1;          /* 2^0 */

    /* multiply by 2 k times; works for all seq_t types that support *
       and constants of type seq_t. */
    for (idx_t i = 0; i < k; ++i)
        pow2 = pow2 * (seq_t)2;

    return pow2 * (seq_t)45;
}
