/*
 * A163577  Count of indices x in [0,n] that satisfy the equation A000120(x) +
 *          A000120(n-x) = A000120(n) + 2.
 * https://oeis.org/A163577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = count of integers x ∈ [0,n] such that popcount(x) +
 *            popcount(n−x) = popcount(n) + 2 (OEIS A163577)
 * Range:     u8 n <= 638, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    unsigned int pc_n = __builtin_popcountll((unsigned long long)n);
    for (idx_t x = 0; x <= n; ++x)
    {
        unsigned int pc_x  = __builtin_popcountll((unsigned long long)x);
        unsigned int pc_nx = __builtin_popcountll((unsigned long long)(n - x));
        if (pc_x + pc_nx == pc_n + 2)
            ++count;
    }
    return count;
}
