/*
 * A121944  Composite number of the form 4n^2+1.
 * https://oeis.org/A121944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121944: Composite numbers of the form 4n^2+1.
   We generate the n-th composite of 4m^2+1 by iterating positive
   integers m and selecting those for which 4*m*m+1 is not prime. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of composites found so far     */
    for (idx_t m = 1; ; ++m) /* iterate all positive m                */
    {
        /* Compute 4*m*m+1 in the integer type guaranteed to hold the value.  */
        seq_t val = ((seq_t)4 * m * m) + 1;

        /* Skip if the value is prime.  */
        if (!is_prime_big(val))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
