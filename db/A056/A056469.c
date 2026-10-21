/*
 * A056469  Number of elements in the continued fraction for Sum_{k=0..n}
 *          1/2^2^k.
 * https://oeis.org/A056469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n‑1) + 2 (with a(0) = 2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t val = (seq_t)1;            /* 2^0 */
    for (idx_t i = 1; i < n; ++i)    /* compute 2^(n-1) by doubling */
    {
        val += val;                  /* val = val * 2 */
    }

    return val + ((seq_t)2);
}
