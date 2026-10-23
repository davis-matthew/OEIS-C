/*
 * A194820  Number of integers k in [1,n] such that {n*e+k*e} > {n*e-k*e},
 *          where { } = fractional part.
 * https://oeis.org/A194820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k ∈ [1,n] | {e·(n+k)} > {e·(n−k)}}, where {x} = x − ⌊x⌋
 *            is the fractional part
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Euler's number with long double precision.  */
    static const long double E = 2.7182818284590452353602874713527L;

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute the two fractional parts using truncation to integer. */
        long double v1 = E * ((long double)n + (long double)k);
        long double v2 = E * ((long double)n - (long double)k);

        long double f1 = v1 - (long long)v1;
        long double f2 = v2 - (long long)v2;

        if (f1 > f2)
            ++count;
    }
    return count;
}
