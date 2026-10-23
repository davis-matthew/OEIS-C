/*
 * A151793  Partial sums of A151782.
 * https://oeis.org/A151793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=2}^{n} 8·7^{popcount(k-1)−1} , a(1)=1 where
 *            popcount(x) is the number of 1‑bits in the binary representation
 *            of x
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1975, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* outside the offset, return 0 */

    seq_t sum = (seq_t)1;          /* a(1) */
    if (n == (idx_t)1)
        return sum;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        idx_t x  = i - (idx_t)1;  /* we need popcount of (k-1) */
        unsigned pc = __builtin_popcountll((unsigned long long)x); /* 1 <= pc <= 64 */

        seq_t term = (seq_t)8;
        /* exponent is pc-1, so multiply by 7 that many times */
        for (unsigned j = 1; j < pc; ++j)
            term *= (seq_t)7;

        sum += term;
    }
    return sum;
}
