/*
 * A166080  Nonprimes of the form (k^2+1)/2.
 * https://oeis.org/A166080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (k^2+1)/2 where k is the n‑th odd integer for which
 *            (k^2+1)/2 is composite (or equals 1)
 * Range:     u8 n <= 3, u16 n <= 99, u32 n <= 154, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1, return 0 as a safety value */

    idx_t count = 0;          /* how many composite terms we have seen */
    idx_t k = 1;              /* current odd integer to test */

    for (;;)
    {
        /* Compute (k^2+1)/2 safely in seq_t arithmetic */
        seq_t k_sq   = (seq_t)k * (seq_t)k;
        seq_t value  = (k_sq + 1) / 2;

        /* value == 1 or composite? 1 is not prime, so just test primality */
        if (!is_prime_big(value))
        {
            ++count;
            if (count == n)
                return value;
        }

        k += 2;                /* next odd integer */
    }
}
