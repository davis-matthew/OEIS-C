/*
 * A142973  a(n) = the smallest positive integer that does not divide any
 *          integer k, where the n-th prime <= k <= the (n+1)th prime.
 * https://oeis.org/A142973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m that does not divide any
 *            integer k with the n-th prime <= k <= the (n+1)-th prime
 * Range:     u8 n <= 53, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for indices below the offset */
        return 0;
    }

    /* Obtain the n-th and (n+1)-th primes */
    uint32_t p_n   = get_prime(n);     /* 1‑indexed */
    uint32_t p_n1  = get_prime(n + 1);

    seq_t m = 1;

    /* Search for the smallest m with no multiple in [p_n, p_n1] */
    while (1) {
        seq_t a_start = (seq_t)p_n;
        seq_t a_end   = (seq_t)p_n1;

        /* First multiple of m that is >= a_start */
        seq_t first_multiple = ((a_start + m - 1) / m) * m;

        if (first_multiple > a_end) {
            /* No multiple of m appears in the interval */
            return m;
        }
        m++;
    }
}
