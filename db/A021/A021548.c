/*
 * A021548  Decimal expansion of 1/544.
 * https://oeis.org/A021548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 544) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) modulo 5440 = 544 * 10.
       Then the desired digit is (10^(n+1) mod 5440) / 544. */
    const unsigned long long MOD = 5440ULL;      /* 544 * 10 */
    const unsigned long long DIV = 544ULL;

    idx_t exp = n;
    exp++;                                       /* exponent = n+1 */

    unsigned long long result = 1ULL;
    unsigned long long base   = 10ULL % MOD;

    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    return (seq_t)(result / DIV);
}
