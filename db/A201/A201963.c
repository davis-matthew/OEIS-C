/*
 * A201963  Primes of the form 10n^2 - 7.
 * https://oeis.org/A201963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 10*n^2 - 7 (OEIS A201963)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 1345, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;          /* variable that runs through the values of n in 10*n^2 - 7 */

    while (count < n) {
        /* Compute 10*m*m - 7 in seq_t to avoid overflow */
        seq_t mm = (seq_t)m * (seq_t)m;
        seq_t p  = (seq_t)10 * mm - 7;

        /* Test primality */
        if (is_prime_uint64((uint64_t)p)) {
            count++;
            if (count == n)
                return p;
        }
        m++;
    }

    /* This point should never be reached for valid n */
    return (seq_t)0;
}
