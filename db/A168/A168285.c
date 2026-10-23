/*
 * A168285  a(n) = ((n-th nonprime)-th prime) - (n-th nonprime).
 * https://oeis.org/A168285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A018252(n)) - A018252(n)
 * Range:     u8 n <= 48, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The A018252 sequence is the list of non‑prime numbers (starting with 1). */
/* For each n we find the n‑th nonprime x, then obtain the x‑th prime via */
/* the helper get_prime(k).  The difference is returned as seq_t. */

#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Find the n‑th nonprime (A018252) */
    idx_t count = 0;
    idx_t x = 1;          /* nonprime list starts at 1 */
    idx_t np = 0;         /* value of the n‑th nonprime */

    while (1) {
        if (!is_prime(x)) {
            ++count;
            if (count == n) {
                np = x;
                break;
            }
        }
        ++x;
    }

    /* Obtain the np‑th prime using the provided helper */
    uint32_t prime = get_prime(np);

    /* Compute the required difference */
    seq_t result = (seq_t)prime - (seq_t)np;
    return result;
}
