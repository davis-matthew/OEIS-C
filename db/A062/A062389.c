/*
 * A062389  a(n) = floor( (2n-1)*Pi/2 ).
 * https://oeis.org/A062389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n-1)*Pi/2)
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a rational approximation of Pi:
     *     Pi ≈ 3141592653589793 / 1000000000000000
     * The error is < 2·10⁻¹⁶, so for any n the
     * resulting floor value is unchanged. */
    static const unsigned __int128 PI_NUM = (unsigned __int128)3141592653589793ULL;
    static const unsigned __int128 PI_DEN = (unsigned __int128)1000000000000000ULL;

    unsigned __int128 nn      = n;                /* n as 128‑bit integer */
    unsigned __int128 two_n_m1 = (nn << 1) - 1;   /* 2n‑1 */
    unsigned __int128 num      = two_n_m1 * PI_NUM;
    unsigned __int128 denom    = PI_DEN << 1;     /* 2·PI_DEN */

    unsigned __int128 res128  = num / denom;      /* floor division */

    return (seq_t)res128;
}
