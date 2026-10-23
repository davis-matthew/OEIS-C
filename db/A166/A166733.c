/*
 * A166733  Numbers n with the property that the concatenation of the trivial
 *          divisors of n (i.e., 1 and n) is a prime.
 * https://oeis.org/A166733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166733: Numbers n where the concatenation of the trivial
   divisors 1 and n (i.e., "1" followed by decimal n) is a prime. */
/* Implements: a(n) = the nth positive integer n>1 satisfying
   is_prime_big(1*10^digits(n) + n). */

#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* Undefined for n = 0; return 0 safely */

    idx_t count   = 0;
    idx_t candidate = 3;          /* n starts from 3 (1 and 2 are not valid) */

    while (true) {
        /* Number of decimal digits of candidate */
        int d = digits(candidate);

        /* Concatenate 1 and candidate: 1*10^d + candidate */
        seq_t concat = (seq_t)pow10((idx_t)d) + (seq_t)candidate;

        /* Test primality of the concatenated number */
        if (is_prime_big(concat)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* Found the nth term */
        }
        ++candidate;                     /* Check next integer */
    }
}
