/*
 * A102750  Numbers n such that square of largest prime dividing n does not
 *          divide n.
 * https://oeis.org/A102750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 84 terms through n = 8691 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102750: Numbers n such that the largest prime factor of n has exponent 1
   (i.e. the square of the largest prime dividing n does not divide n).
   The function iterates through integers, tests the condition by factorisation
   with trial division, and returns the n-th such number. */

#include <stdbool.h>

static bool good(seq_t v)
{
    seq_t temp = v;
    seq_t last_exp = 0;

    /* Factorise by trial division */
    for (seq_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            seq_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            /* For each prime factor we record its exponent.  The last
               factor processed that divides 'temp' will always be the
               largest prime exponent we encounter so far. */
            last_exp = exp;
        }
    }

    /* Any remaining factor is a prime larger than all previous factors. */
    if (temp > 1)
        last_exp = 1;

    return last_exp == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    seq_t value = 1;                /* start before first candidate */
    idx_t count = 0;

    while (count < n)
    {
        ++value;                    /* candidate integer */
        if (good(value))
            ++count;
    }

    return value;                   /* n-th term */
}
