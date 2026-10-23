/*
 * A028888  Smaller of two successive primes with a palindromic product.
 * https://oeis.org/A028888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = smaller prime p such that p and the next prime q have p*q
 *            a decimal palindrome
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_palindrome, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence is defined for n >= 1 only */

    seq_t result = 0;
    idx_t found   = 0;
    uint64_t p    = 2;              /* start with the first prime */

    while (found < n) {
        uint64_t q = next_prime(p);                /* successive prime */
        seq_t prod = (seq_t)p * (seq_t)q;

        if (is_palindrome(prod, 10)) {             /* palindrome in base 10 */
            ++found;
            if (found == n) {
                result = (seq_t)p;                 /* return the smaller prime */
                break;
            }
        }
        p = q;                                      /* move to next pair */
    }

    return result;
}
