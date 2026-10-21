/*
 * A096268  Period-doubling sequence (or period-doubling word): fixed point of
 *          the morphism 0 -> 01, 1 -> 00.
 * https://oeis.org/A096268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the exponent of 2 dividing n+1 is odd, otherwise 0 *
 *            (equivalently a(2*n) = 0, a(4*n+1) = 1, a(4*n+3) = a(n) with
 *            a(0)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n+1, guarding against overflow */
    idx_t t = n + 1ULL;
    /* count trailing zeros of t to get the exponent of 2 dividing n+1 */
#if defined(__GNUC__) || defined(__clang__)
    unsigned int tz = __builtin_ctzll(t);   /* fast builtin on GCC/Clang */
#else
    unsigned int tz = 0;
    while ((t & 1ULL) == 0ULL) {
        t >>= 1;
        ++tz;
    }
#endif
    return (seq_t)(tz & 1);   /* 1 if odd, 0 if even */
}
