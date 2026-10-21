/*
 * A211664  Number of iterations (...(log_4(log_3(log_2(n))))...) such that
 *          the result is < 1.
 * https://oeis.org/A211664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211664: a(n) = number of iterations of
   successive logarithms log_2, log_3, log_4, … applied
   to n until the result becomes < 1.  The value returned
   includes the last logarithm that produced the < 1 result. */

#include <math.h>
#include <stdbool.h>   /* for bool */

seq_t a(idx_t n)
{
    /* Special case: n == 1 trivially satisfies log_2(1) < 1 */
    if (n == 1)
        return 1;

    /* Use double to carry out the logarithmic recursions.
       For n up to at least 10^12 this is precise enough for
       the comparison x < base.  The sequence grows extremely
       fast so only a few iterations are ever required. */
    double x = (double)n;
    unsigned int base = 2;   /* current log base (incrementing each step) */
    seq_t count = 0;         /* number of iterations performed so far */

    while (1) {
        if (x < (double)base)    /* the current result is already < base,
                                   i.e. log_{base}(x) < 1 */
            return count + 1;    /* include the final step in the count */

        /* Compute the next value in the nested logarithm chain. */
        x = log(x) / log((double)base);

        /* Advance to the next logarithm base. */
        base++;
        count++;
    }
}
