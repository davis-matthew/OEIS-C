/*
 * A135983  a(n)=2^A107006(n)-1.
 * https://oeis.org/A135983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p - 1 where p is the n‑th prime ≡ 7 (mod 24)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/pow_uint.h"

/* Return the nth term of OEIS A135983.
   n is 1‑based.  For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    /* invalid index */
    if (n == 0)
        return 0;

    uint64_t found   = 0;          /* number of found primes of the form 24k+7 */
    uint64_t candidate = 7;        /* first number of the form 24k+7  */
    uint64_t prime_of_form = 0;    /* target prime */

    /* Scan candidates of the form 24k+7 until we hit the nth prime.  */
    while (found < n) {
        if (is_prime(candidate)) {
            ++found;
            if (found == n) {
                prime_of_form = candidate;
                break;
            }
        }
        candidate += 24;           /* jump to next 24k+7 number */
    }

    /* Compute 2^prime_of_form - 1.  pow_uint returns a seq_t. */
    seq_t term = pow_uint(2, prime_of_form);
    term = term - (seq_t)1;        /* subtract 1 */

    return term;
}
