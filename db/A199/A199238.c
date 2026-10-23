/*
 * A199238  n mod (number of ones in binary representation of n).
 * https://oeis.org/A199238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (number of ones in binary representation of n)
 * Range:     u8 n <= 469, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t popcount_idx(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)  /* GCC/Clang built‑in */
    return __builtin_popcountll(x);
#else
    /* Portable popcount implementation */
    idx_t c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    /* n is >= 1; popcount(n) >= 1 */
    idx_t ones = popcount_idx(n);
    if (ones == 0)  /* safety guard, should never happen for n >= 1 */
        return 0;
    return (seq_t)n % (seq_t)ones;
}
