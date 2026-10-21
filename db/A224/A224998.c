/*
 * A224998  Floor(1/f(x^(1/n))) for x = Pi, where f computes the fractional
 *          part.
 * https://oeis.org/A224998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n / log(Pi) - 1/2 ) for n > 4 with a(1)=7, a(2)=1,
 *            a(3)=2, a(4)=3. 1/log(Pi) ≈ 873216/1000000 with absolute error <
 *            1.2·10⁻⁷, which guarantees the floor is unchanged for n ≤ 10000
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base_vals[4] = {7, 1, 2, 3};

seq_t a(idx_t n)
{
    if (n <= 4)
        return base_vals[n - 1];

    /* Use (numerator, denominator) = (873216, 1000000) ≈ 1/log(Pi) */
    const unsigned __int128 N = 873216;   /* numerator  */
    const unsigned __int128 D = 1000000;  /* denominator */

    /* Compute floor( (2*n*N - D) / (2*D) ) */
    unsigned __int128 num = (unsigned __int128)2 * n * N;
    /* num is always ≥ D for n ≥ 5, but guard against underflow */
    if (num < D) return 0;
    num -= D;
    unsigned __int128 den = (unsigned __int128)2 * D;

    return (seq_t)(num / den);
}
