/*
 * A010965  a(n) = binomial(n,12).
 * https://oeis.org/A010965
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,12) for n >= 12
 * Range:     u8 n <= 14, u16 n <= 19, u32 n <= 39, u64 n <= 218,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 12)
        return 0;                       /* below the offset – unused */
    const idx_t k = 12;                 /* fixed upper argument */

    /* Use a 128‑bit type for the intermediate computation.
       __uint128_t is a GCC/Clang extension that is available when building
       with -std=c11 or -std=c++17.  The final result fits in seq_t by
       assumption of the build. */
    __uint128_t res = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (__uint128_t)(n - k + i) / (__uint128_t)i;
    }

    return (seq_t)res;
}
