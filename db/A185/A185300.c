/*
 * A185300  Numbers k such that (sum of the decimal digits of k) + (product of
 *          the decimal digits of k) is prime.
 * https://oeis.org/A185300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer k ≥ 1 for which (sum of decimal digits of
 *            k) + (product of decimal digits of k) is a prime number
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Return the nth term of OEIS A185300.
   The function is memoization‑free and works for any build
   (C or C++), with seq_t being a plain integer type or an
   arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n = 0; return 0 for safety */
        return 0;

    idx_t   k   = 1;      /* current candidate integer */
    idx_t   cnt = 0;      /* number of good k found so far */

    /* Scan integers until the nth good one is found */
    while (1) {
        seq_t sum = 0;    /* running sum of digits */
        seq_t prod= 1;    /* running product of digits */

        idx_t t = k;
        while (t > 0) {
            int d = t % 10;       /* next digit */
            sum += (seq_t)d;      /* add digit to sum */
            prod *= (seq_t)d;      /* multiply digit to product */
            t /= 10;
        }

        if (is_prime_small(sum + prod)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)k;
        }
        ++k;   /* test next integer */
    }
}
