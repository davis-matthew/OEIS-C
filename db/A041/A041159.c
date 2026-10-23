/*
 * A041159  Denominators of continued fraction convergents to sqrt(89).
 * https://oeis.org/A041159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1000*a(n-5) + a(n-10) Initial values: a(0)=1, a(1)=2,
 *            a(2)=7, a(3)=23, a(4)=53, a(5)=977, a(6)=2007, a(7)=6998,
 *            a(8)=23001, a(9)=53000
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base terms for n = 0 .. 9 */
    static const seq_t base[10] = {
        1ULL, 2ULL, 7ULL, 23ULL, 53ULL,
        977ULL, 2007ULL, 6998ULL, 23001ULL, 53000ULL
    };

    if (n < 10) {
        return base[n];
    }

    /* Circular buffer holding the last 10 computed values */
    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i) {
        buf[i] = base[i];
    }

    /* Compute up to a(n) using the recurrence */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t val = 1000 * buf[(i - 5) % 10] + buf[(i - 10) % 10];
        buf[i % 10] = val;
    }

    return buf[n % 10];
}
