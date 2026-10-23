/*
 * A185048  Length of the continued fraction for
 *          floor(Fibonacci(n)*(1+sqrt(5))/2) / Fibonacci(n).
 * https://oeis.org/A185048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=1; for k≥2, a(2k)=2*⌊k/2⌋, a(2k-1)=2k-2
 * Range:     u8 n <= 256, u16 n <= 9547, u32 n <= 9547, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;
    if (n & 1)            /* odd n ≥ 3: a(n) = n-1 */
        return (seq_t)(n - 1);
    /* even n ≥ 4: n = 2k → a = 2*⌊k/2⌋ = 2*⌊n/4⌋ */
    return (seq_t)(2 * ((seq_t)(n / 4)));
}
