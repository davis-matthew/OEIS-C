/*
 * A159699  Replace 2^k in binary expansion of n with A045623(k+1).
 * https://oeis.org/A159699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = Σ_{k≥1, bit_k(n)=1} k·2^{k-1} + 2·n (with k starting at
 *            0)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t orig = n;
    /* Iterate over bits of n */
    for (idx_t k = 0; n; ++k, n >>= 1)
    {
        if (n & 1)
        {
            if (k) /* k = 0 contributes 0 anyway */
            {
                /* k * 2^{k-1} is implemented as (k << (k-1)) */
                result += ((seq_t)k) << (k - 1);
            }
        }
    }
    result += (seq_t)2 * (seq_t)orig;
    return result;
}
