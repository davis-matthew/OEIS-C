/*
 * A020513  Cyclotomic polynomials evaluated at x=-1.
 * https://oeis.org/A020513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 261, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = -1
 *          a(1) = -2
 *          a(2) =  0
 *          a(n) = p           if n = 2 * p^m  (p prime, m ≥ 1)
 *          a(n) = 1           otherwise
 *
 * This implementation follows the definition directly.  For n ≥ 3 it
 * checks whether n is even; if so it tests whether n/2 is a prime power
 * and, if so, returns the base prime.  The check uses a simple trial
 * division for the smallest prime factor, which is fast for the size
 * limits required in the tests (n up to about 10 000 or a few
 * million).  No external helper functions are needed, so the build
 * links cleanly for any seq_t/idx_t combination.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special first three values */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)0;

    /* For n ≥ 3 */
    if ((n & 1U) == 0)       /* even n */
    {
        uint64_t t = n >> 1; /* t = n/2 */

        /* Find smallest prime factor of t */
        uint64_t p = 0;
        if (t % 2 == 0)
            p = 2;
        else
        {
            for (uint64_t d = 3; d <= t / d; d += 2)
            {
                if (t % d == 0)
                {
                    p = d;
                    break;
                }
            }
            if (p == 0)  /* t is prime */
                p = t;
        }

        /* Strip all copies of p from t */
        while (t % p == 0)
            t /= p;

        if (t == 1)   /* t was a prime power */
            return (seq_t)p;
    }

    /* All other cases return 1 */
    return (seq_t)1;
}
