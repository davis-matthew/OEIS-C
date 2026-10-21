/*
 * A052488  a(n) = floor(n*H(n)) where H(n) is the n-th harmonic number,
 *          Sum_{k=1..n} 1/k (A001008/A002805).
 * https://oeis.org/A052488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n * H(n)) where H(n) = sum_{k=1..n} 1/k
 * Range:     u8 n <= 55, u16 n <= 6953, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The harmonic number H(n) is computed in long double because
       the exact value would require a rational with enormous
       denominator.  The final result is an integer that fits in a
       native 64‑bit type for all n that fit in seq_t.  We cast to
       uint64_t first, then to seq_t, which works for builds where
       seq_t is a big integer class as well. */

    if (n == 0)                 /* offset is 1; n=0 is out of range */
        return (seq_t)0;

    long double h = 0.0L;
    /* accumulate the harmonic series in O(n) time */
    for (idx_t k = 1; k <= n; ++k)
        h += 1.0L / (long double)k;

    long double prod = (long double)n * h;
    /* truncation of a positive long double is the floor */
    uint64_t val = (uint64_t)prod;
    return (seq_t)val;
}
