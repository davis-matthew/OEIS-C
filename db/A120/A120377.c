/*
 * A120377  Primes of the form 2*11^k-1.
 * https://oeis.org/A120377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 2*11^k - 1 (OEIS A120377) */
/* Computation is performed by enumerating k=1,2,... and testing primality
   of 2*11^k - 1 until the n-th such prime is found. */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n>=1. Return 0 for safety. */
        return (seq_t)0;
    }

    seq_t result = (seq_t)0;
    idx_t found = 0;
    idx_t k = 1;                /* current exponent */

    while (found < n) {
        /* Compute p = 2*11^k - 1 */
        seq_t pow_val = big_pow((seq_t)11, (seq_t)k);   /* 11^k */
        pow_val = pow_val * (seq_t)2;                   /* 2*11^k */
        pow_val = pow_val - (seq_t)1;                   /* 2*11^k - 1 */

        if (is_prime_big(pow_val)) {                    /* test primality */
            found++;
            if (found == n) {
                result = pow_val;
                break;
            }
        }
        k++;
    }
    return result;
}
