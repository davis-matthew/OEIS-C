/*
 * A093462  a(1)=1, a(n) = 2(n^(n-1)-1)/(n-1)^2.
 * https://oeis.org/A093462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n^(n-1)-1)/(n-1)^2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 1U)
        return (seq_t)1;          /* a(1) = 1 */

    /* (n-1) as an intermediate of sufficient width */
    unsigned __int128 denom_part = (unsigned __int128)(n - 1U);

    /* Compute n^(n-1) in an unsigned __int128 register */
#if defined(__cplusplus)
    /* C++ path – use unsigned __int128 for the whole calculation */
    unsigned __int128 power = 1;
    unsigned __int128 base   = (unsigned __int128)n;
    for (idx_t i = 1; i < n; ++i)
        power *= base;

    /* numerator = 2*(power - 1) */
    unsigned __int128 numerator = (power - 1U) * 2U;
    unsigned __int128 denom = denom_part * denom_part;
    unsigned __int128 result128 = numerator / denom;
    return (seq_t)result128;
#else
    /* C path – the same unsigned __int128 arithmetic */
    unsigned __int128 power = 1;
    unsigned __int128 base   = (unsigned __int128)n;
    for (idx_t i = 1; i < n; ++i)
        power *= base;

    unsigned __int128 numerator = (power - 1U) * 2U;
    unsigned __int128 denom = denom_part * denom_part;
    unsigned __int128 result128 = numerator / denom;
    return (seq_t)result128;
#endif
}
