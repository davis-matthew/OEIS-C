/*
 * A024906  Numbers k such that 9*k + 1 is prime.
 * https://oeis.org/A024906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 837, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 9*k + 1 is prime (OEIS A024906) */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Keep state between calls so that repeated, unordered requests
       are efficient. */
    static idx_t found = 0;      /* how many terms found so far   */
    static seq_t k = 0;          /* current candidate k          */

    /* n will always be >= 1 in the tests.  A guard for n == 0 is harmless. */
    if (n <= 0)
        return 0;

    while (found < n) {
        k += 1;                  /* try next candidate */
        seq_t p = k;
        p *= 9;
        p += 1;                  /* p = 9 * k + 1          */
        if (is_prime_big(p))
            found++;             /* k is a term of the sequence */
    }
    return k;
}
