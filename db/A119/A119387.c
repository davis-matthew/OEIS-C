/*
 * A119387  a(n) is the number of binary digits (1's and nonleading 0's) which
 *          remain unchanged in their positions when n and (n+1) are written
 *          in binary.
 * https://oeis.org/A119387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = floor(log2(n+1)) - valuation(n+1,2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where valuation(x,2) is the exponent of 2 in the prime factorisation of x. */

seq_t a(idx_t n)
{
    /* n is unsigned; compute m = n+1 safely */
    idx_t m = n + 1;
    if (m == 0)            /* overflow when n == UINT64_MAX */
        return (seq_t)0;

    /* Highest set bit position (0‑based) */
    int msb = 63 - __builtin_clzll(m);

    /* Number of trailing zeros */
    int tz  = __builtin_ctzll(m);

    /* Result fits in seq_t because msb <= 63 and tz <= msb */
    return (seq_t)((seq_t)msb - (seq_t)tz);
}
