/*
 * A053167  Smallest 4th power divisible by n.
 * https://oeis.org/A053167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^(e + ((4−e) mod 4))
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        = ∏_{p^e‖n} p^(((e+3)/4)*4)  (rounded exponent up to the next multiple of 4) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: fast exponentiation for any seq_t type */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Handle factor 2 separately */
    if ((m & 1) == 0)
    {
        idx_t e = 0;
        while ((m & 1) == 0)
        {
            ++e;
            m >>= 1;
        }
        idx_t f = ((e + 3) / 4) * 4;   /* round up to next multiple of 4 */
        result *= pow_uint((seq_t)2, f);
    }

    /* Trial division over odd divisors */
    for (idx_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                ++e;
                m /= p;
            }
            idx_t f = ((e + 3) / 4) * 4;
            result *= pow_uint((seq_t)p, f);
        }
    }

    /* If a prime factor > sqrt(original n) remains */
    if (m > 1)
    {
        /* Remaining factor appears to first power, so exponent = 4 */
        result *= pow_uint((seq_t)m, 4);
    }

    return result;
}
