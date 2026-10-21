/*
 * A056117  Expansion of (1+8*x)/(1-x)^9.
 * https://oeis.org/A056117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*n+8)*C(n+7,7)/8 (OEIS A056117)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 42, u64 n <= 728, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+7,7) with a 128‑bit intermediate to avoid overflow.
       For idx_t up to 10000 the intermediate stays well below 2^128.  */
    unsigned __int128 binom = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        binom = (binom * ((unsigned __int128)n + i)) / i;
    }

    /* Final term = (9*n + 8) * binom / 8.  Use the same 128‑bit type
       for the intermediate product.  Result fits into seq_t for the
       values required by the test harness.  */
    unsigned __int128 num  = binom * (((unsigned __int128)9 * n) + 8);
    return (seq_t)(num / 8);
}
