/*
 * A120454  a(n) = ceiling(GPF(n)/LPF(n)) where GPF is greatest prime factor,
 *          LPF is least prime factor.
 * https://oeis.org/A120454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceiling(GPF(n)/LPF(n)) where GPF is greatest prime factor
 *            and LPF the least prime factor of n
 * Range:     u8 n <= 625, u16 n <= 9893, u32 n <= 9893, u64 n <= 9893,
 *            u128 n <= 9893, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic and works for any
   integer type seq_t and idx_t defined by the build.  For n = 1 the value
   is defined to be 1.  No external helper functions are required. */

#include <stdint.h>        /* for uint8_t, uint64_t, etc.  Seq_t and idx_t
                            * are defined by the prelude and are not redefined
                            * here. */

/* External linkage */
seq_t a(idx_t n)
{
    /* n >= 1 by specification; handle the trivial case directly. */
    if (n == 1)
        return (seq_t)1;

    /* least and greatest prime factors, initialised to 0 (unset). */
    idx_t lp = 0;   /* least prime factor */
    idx_t gp = 0;   /* greatest prime factor */

    /* m will be repeatedly divided out of n; start with a copy. */
    idx_t m = n;

    /* Trial division up to sqrt(m).  Using m/p as the loop bound avoids
       overflow and keeps the check incremental. */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            /* First divisor found is the least prime factor. */
            if (lp == 0)
                lp = p;

            /* The last divisor encountered will be the largest prime factor. */
            gp = p;

            /* Remove all occurrences of p from m. */
            while (m % p == 0)
                m /= p;
        }
    }

    /* If m > 1 after the loop, m itself is a prime factor > sqrt(original n). */
    if (m > 1)
    {
        if (lp == 0)          /* this happens only when n itself is prime */
            lp = m;
        gp = m;                /* it is also the greatest prime factor */
    }

    /* Compute ceil(gp / lp).  The division is performed in seq_t to avoid
       overflow for very large seq_t values.  lp >= 1 here. */
    seq_t numer = (seq_t)gp;
    seq_t denom = (seq_t)lp;
    seq_t res = numer / denom;
    if (numer % denom != 0)
        ++res;

    return res;
}
