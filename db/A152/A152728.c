/*
 * A152728  a(n) + a(n+1) + a(n+2) = n^3.
 * https://oeis.org/A152728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n^3-3*n^2+n)/3) Equivalent piecewise form: a(3k) =
 *            k*(9k^2-9k+1) a(3k+1) = k*(9k^2-2) a(3k+2) = k*(9k^2+9k+1)
 * Range:     u8 n <= 10, u16 n <= 59, u32 n <= 2345, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Split n into quotient and remainder of division by 3. */
    idx_t k = n / 3;
    idx_t r = n % 3;

    /* Use a wider unsigned type for intermediate calculations to avoid
       overflow when seq_t is narrow (e.g. uint8_t, uint16_t). */
    unsigned long long K = (unsigned long long)k;
    unsigned long long t = 9ULL * K * K;  /* 9*k^2 */

    if (r == 0)          /* a(3k)   = k*(9k^2-9k+1) */
        t = t - 9ULL * K + 1;
    else if (r == 1)     /* a(3k+1) = k*(9k^2-2)   */
        t = t - 2;
    else                 /* a(3k+2) = k*(9k^2+9k+1) */
        t = t + 9ULL * K + 1;

    unsigned long long val = K * t;
    return (seq_t)val;
}
