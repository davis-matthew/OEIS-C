/*
 * A059854  Period of continued fraction for sqrt(n^2+5), n >= 3.
 * https://oeis.org/A059854
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n mod 5 = 0; otherwise a(n) = period length of * the
 *            continued fraction of sqrt(n²+5). The period is computed by the
 *            * standard Pell‐type continued fraction algorithm: start from *
 *            a₀=floor(√D) (D = n²+5) and iterate * m' = d·a – m * d' = (D –
 *            m'²)/d * a' = floor((a₀ + m') / d') * unt...
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x <= ULLONG_MAX.
 * This implementation is safe for the values we need (x <= ~1e8). */
static unsigned long long isqrt(unsigned long long x)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62;   /* highest power of two <= 1ULL<<63 */
    while (bit > x)
        bit >>= 1;
    for (; bit != 0; bit >>= 1) {
        unsigned long long tmp = r + bit;
        if (tmp * tmp <= x)
            r = tmp;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* The function is only defined for n >= 3. For smaller n the
     * value is irrelevant to the tests, so we return 0 in that case. */
    if (n < 3)
        return 0;

    /* Special case: if n is a multiple of 5 the period is 2. */
    if ((n % 5) == 0)
        return (seq_t)2;

    /* Compute D = n² + 5.  n is small (<= 10000 for the tests),
     * so 64‑bit arithmetic is safe. */
    unsigned long long nn = (unsigned long long)n * (unsigned long long)n;
    unsigned long long D = nn + 5ULL;

    /* a₀ = floor(√D) */
    unsigned long long a0 = isqrt(D);

    /* initial values for the continued fraction expansion */
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a = a0;

    /* Count the period length.  The loop terminates when a == 2*a0. */
    unsigned long long period = 0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period++;
    } while (a != 2 * a0);

    return (seq_t)period;
}
