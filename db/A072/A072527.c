/*
 * A072527  Number of values of k such that n divided by k leaves a remainder
 *          3.
 * https://oeis.org/A072527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = τ(n-3) - (1 + [(n-3)%2==0] + [(n-3)%3==0]) for n > 3,
 *            a(n) = 0 for n ≤ 3. Here τ(m) is the number of divisors of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Return the divisor count of an unsigned 64‑bit integer.  */
static unsigned long long tau_uint64(unsigned long long x)
{
    unsigned long long tau = 1;
    unsigned long long tmp  = x;
    for (unsigned long long p = 2; p * p <= tmp; ++p)
    {
        if ((tmp % p) == 0)
        {
            unsigned long long exp = 0;
            while ((tmp % p) == 0)
            {
                ++exp;
                tmp /= p;
            }
            tau *= (exp + 1);
        }
    }
    if (tmp > 1)  /* remaining prime factor */
        tau *= 2;
    return tau;
}

seq_t a(idx_t n)
{
    /* values ≤ 3 are defined to be 0 */
    if (n <= 3ULL)
        return 0;

    /* m = n - 3  (unsigned subtraction safe after the check above) */
    uint64_t m = (uint64_t)n - 3ULL;

    /* τ(m) */
    unsigned long long tau = tau_uint64(m);

    /* subtract small divisors 1, 2, 3 if they divide m */
    seq_t result = (seq_t)tau;
    result -= 1;                      /* subtract divisor 1 */
    if ((m & 1ULL) == 0)              /* divisor 2 */
        result -= 1;
    if ((m % 3ULL) == 0)              /* divisor 3 */
        result -= 1;

    return result;
}
