/*
 * A095202  Value of largest k such that (n-1) + (n-2) + (n-3) + ... + (n-k)
 *          is a multiple of n, or 0 if no such k exists.
 * https://oeis.org/A095202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k < n such that (n-1)+(n-2)+...+(n-k) ≡ 0 (mod
 *            n), or 0 if no such k exists. Equivalently, k*(2n-k-1) ≡ 0 (mod
 *            2n)
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* n is at least 1 by definition; for n <= 1 no k exists */
    if (n <= 1U)
        return 0;

    /* Convert index to seq_t for arithmetic. */
    seq_t S = (seq_t)n;
    seq_t mod = (seq_t)2 * S;   /* 2n, divisor for the congruence */

    /* Iterate downward from n-1 to 1 to find the largest valid k. */
    for (idx_t k = n - 1U; k > 0U; --k)
    {
        /* Compute k * (2n - k - 1) as seq_t to avoid overflow on idx_t. */
        seq_t t = (seq_t)k * (((seq_t)2 * S) - (seq_t)k - (seq_t)1);

        /* Check the congruence. */
        if (t % mod == 0)
            return (seq_t)k;
    }

    /* No valid k found. */
    return 0;
}
