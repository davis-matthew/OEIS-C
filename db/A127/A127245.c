/*
 * A127245  Row sums of a signed Thue-Morse related triangle.
 * https://oeis.org/A127245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if popcount(n) is even, otherwise a(n) = popcount(n-1)
 *            mod 2. Derived from a(n) = 1 - A010060(n)*A010059(n-1) with
 *            A010060(n) = parity of popcount(n) (Thue–Morse) and A010059(n) =
 *            1 - A010060(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* n fits in idx_t (uint64_t) */
    unsigned long long pc_n  = __builtin_popcountll((unsigned long long)n);
    unsigned long long pc_n1 = __builtin_popcountll((unsigned long long)(n - 1ULL));

    if ((pc_n & 1ULL) == 0ULL)
        return (seq_t)1;
    else
        return (seq_t)(pc_n1 & 1ULL);
}
