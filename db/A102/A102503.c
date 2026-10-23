/*
 * A102503  Primes of the concatenated form 9nn9.
 * https://oeis.org/A102503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the nth prime of the form 9nn9 (concatenation of 9, n, n,
 *            9)
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 2500, u128 n <= 2500,
 *            big any n
 * Helpers:   count_digits, is_prime, isprime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime.h"
#include "../utils/isprime_big.h"

seq_t a(idx_t target)
{
    if (target == 0)
        return 0;

    idx_t count = 0;      /* number of primes found so far   */
    idx_t n = 10;         /* the n in the concatenation; n>=10 */

    while (1) {
        /* number of decimal digits of n */
        int d = count_digits(n);

        /* 10^d and 10^(d+1) */
        seq_t pow10_d  = pow10(d);       /* 10^d          */
        seq_t pow10_d1 = pow10(d + 1);   /* 10^(d+1)      */

        /* term = 9*10^(2d+1) + n*(10^(d+1)+10) + 9
           = 9*(pow10_d1 * pow10_d) + n*(pow10_d1 + 10) + 9   */
        seq_t term = 9 * (pow10_d1 * pow10_d) +
                     (seq_t)n * (pow10_d1 + 10) + 9;

        /* primality test : small numbers use the fast 64‑bit routine */
        bool prime;
        if (term <= (seq_t)UINT64_MAX) {
            prime = is_prime((uint64_t)term);
        } else {
            prime = isprime_big(term);
        }

        if (prime) {
            ++count;
            if (count == target)
                return term;
        }
        ++n;
    }
}
