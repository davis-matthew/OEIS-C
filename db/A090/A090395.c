/*
 * A090395  Denominator of d(n)/n, where d(n) is the number of divisors of n
 *          (A000005).
 * https://oeis.org/A090395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = n / gcd(n, d(n)) where d(n) is the number of divisors of
 *            n
 * Range:     u8 n <= 255, u16 n <= 9535, u32 n <= 9535, u64 n <= 9535,
 *            u128 n <= 9535, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – safeguard for 0 input */
        return 0;

    /* --- compute d(n), the divisor count -------------------------------- */
    idx_t temp = n;
    seq_t divs = 1;      /* product of (exponent + 1) for each prime factor */

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            divs *= (exp + 1);
        }
    }
    if (temp > 1)        /* remaining prime factor */
        divs *= 2;

    /* --- compute gcd(n, d(n)) ------------------------------------------- */
    seq_t a_val = n;
    seq_t b_val = divs;
    while (b_val != 0)
    {
        seq_t t = a_val % b_val;
        a_val = b_val;
        b_val = t;
    }
    seq_t g = a_val;      /* gcd */

    /* --- return the denominator of d(n)/n in lowest terms ---------------- */
    return (seq_t)(n / g);
}
