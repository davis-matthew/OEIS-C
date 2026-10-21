/*
 * A079529  a(n) = sigma(n) - ceiling(n + sqrt n) as n runs through the
 *          composite numbers A002808.
 * https://oeis.org/A079529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(c_n) - ceil(c_n + sqrt(c_n)), where c_n is the n‑th
 *            composite number
 * Range:     u8 n <= 44, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_sqrt, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the n-th composite number c_n (c_1 = 4). */
    idx_t count = 0;
    idx_t m = 4;                /* first composite */
    while (count < n)
    {
        if (!is_prime(m))      /* composite */
        {
            ++count;
            if (count == n)
                break;
        }
        ++m;
    }

    /* Compute sigma(c_n). */
    seq_t sigma = sigma_of(m);

    /* Compute ceil(c_n + sqrt(c_n)). */
    seq_t sf   = floor_sqrt((seq_t)m);          /* floor sqrt(c_n)      */
    seq_t ceil_val;
    if (sf * sf == (seq_t)m)                    /* perfect square?      */
        ceil_val = (seq_t)m + sf;
    else
        ceil_val = (seq_t)m + sf + (seq_t)1;    /* ceil value         */

    return sigma - ceil_val;
}
