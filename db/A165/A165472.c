/*
 * A165472  Partial sums of A165471.
 * https://oeis.org/A165472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(n) = floor(n/65537) + sum_{i=1}^{n mod 65537} * ( 1 if i is a
 *            quadratic residue modulo 65537, * -1 otherwise ), a(0) = 0. *
 *            The sequence of residues repeats every 65537 terms, and the sum
 *            of * one full period is 1, hence the linear term floor(n/65537)
 * Range:     i8 n <= 8163, i16 n <= 9486, i32 n <= 9486, i64 n <= 9486,
 *            i128 n <= 9486, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define PERIOD 65537ULL            /* 65537 is prime, period of residues */

/*  a(n) returns the partial sum described above.  */
seq_t a(idx_t n)
{
    /*  On entry n is the index from 0 upwards.  */
    static bool initialized = false;          /* lazy initialization flag */
    static bool is_residue[PERIOD];           /* marks quadratic residues */
    static seq_t cum[PERIOD + 1];             /* cumulative sums, cum[0]=0 */

    if (!initialized)
    {
        /*  Compute the quadratic residue table.  */
        for (idx_t a = 0; a < PERIOD; ++a)
        {
            /* a*a fits in uint64_t for a < PERIOD.  */
            idx_t r = (a * a) % PERIOD;
            is_residue[r] = true;
        }

        /*  Compute cumulative sums for one full period.  */
        cum[0] = 0;
        for (idx_t i = 1; i <= PERIOD; ++i)
        {
            idx_t mod = i % PERIOD;          /* 0..PERIOD-1, with 0 for i==PERIOD */
            seq_t val = is_residue[mod] ? 1 : -1;
            cum[i] = cum[i - 1] + val;
        }

        initialized = true;
    }

    /*  Reduce the index into the stored period and add the
     *  offset contributed by full periods (each contributes +1).  */
    idx_t full_cycles = n / PERIOD;
    idx_t remainder   = n % PERIOD;

    return ((seq_t)full_cycles) + cum[remainder];
}
