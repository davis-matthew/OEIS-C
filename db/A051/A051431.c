/*
 * A051431  a(n) = (n+10)!/10!.
 * https://oeis.org/A051431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+10)! / 10! = product_{k=11}^{n+10} k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t end = n + 10;                /* last factor in the product */
    for (idx_t k = 11; k <= end; ++k)
    {
        result *= (seq_t)k;            /* multiply by the next integer */
    }
    return result;
}
