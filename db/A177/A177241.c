/*
 * A177241  First differences of A177240.
 * https://oeis.org/A177241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 (n=0) ; otherwise a(n) = 4^{popcount(n-1)}
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* popcount of (n-1) */
    uint64_t m = n - 1;
    unsigned int pc = __builtin_popcountll(m);

    seq_t result = 1;
    for (unsigned int i = 0; i < pc; ++i)
        result *= 4;
    return result;
}
