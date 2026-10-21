/*
 * A061087  a(n) = A061086(n) / n.
 * https://oeis.org/A061087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of n and n³ divided by n Simplified: a(n) =
 *            10ⁿ⁺ ⌊log₁₀(n³)⌋ + 1 + n² = 10^{digits(n³)} + n²
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n³ (seq_t to avoid overflow) */
    seq_t cube = (seq_t)n * n * n;

    /* count decimal digits of cube */
    idx_t k = 1;
    seq_t t = cube;
    while (t >= 10)
    {
        t /= 10;
        ++k;
    }

    /* precomputed powers of 10 up to 19 digits (sufficient for n≤10000) */
    static const seq_t POW10[20] = {
        1ULL,
        10ULL,
        100ULL,
        1000ULL,
        10000ULL,
        100000ULL,
        1000000ULL,
        10000000ULL,
        100000000ULL,
        1000000000ULL,
        10000000000ULL,
        100000000000ULL,
        1000000000000ULL,
        10000000000000ULL,
        100000000000000ULL,
        1000000000000000ULL,
        10000000000000000ULL,
        100000000000000000ULL,
        1000000000000000000ULL,
        10000000000000000000ULL
    };
    seq_t pow10;
    if (k < sizeof(POW10) / sizeof(POW10[0]))
    {
        pow10 = POW10[k];
    }
    else
    {
        pow10 = 1;
        for (idx_t i = 0; i < k; ++i)
            pow10 *= 10;
    }

    /* a(n) = pow10 + n² */
    seq_t result = pow10 + (seq_t)n * n;
    return result;
}
