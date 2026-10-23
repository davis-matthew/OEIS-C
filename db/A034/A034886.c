/*
 * A034886  Number of digits in n!.
 * https://oeis.org/A034886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(log10(n!)) + 1
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Kamenetsky's approximation, which is exact for 2 ≤ n ≤ 5·10^7. */

#include <math.h>

seq_t a(idx_t n)
{
    /* Small n can be handled directly. */
    if (n < 2)
        return (seq_t)1;

    /* Compute the Kamenetsky expression using long double for accuracy. */
    const long double PI = 3.141592653589793238462643383279502884L;
    const long double E  = 2.718281828459045235360287471352662497L;

    long double nd = (long double)n;
    long double y  = 0.5L * log10l(2.0L * PI * nd) +
                     nd * (log10l(nd) - log10l(E));
    /* The expression is an integer; ceill gives the exact number of digits. */
    unsigned long long digits = (unsigned long long)ceill(y);

    return (seq_t)(digits);
}
