/*
 * A142229  Primes congruent to 32 mod 41.
 * https://oeis.org/A142229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 156, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A142229: primes congruent to 32 mod 41.  
 * This function finds the n-th such prime by
 * iteratively testing candidates of the form 73 + 41*k.
 * It uses the provided prime test for the seq_t type.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; n >= 1 is required */
        return 0;

    /* start with the first known prime congruent to 32 mod 41 */
    seq_t candidate = 73;
    idx_t count = 1;             /* candidate is already prime */

    if (n == 1)
        return candidate;

    /* iterate over further numbers of the form 73 + 41*k */
    while (true) {
        candidate += 41;  /* next number ≡ 32 (mod 41) */

        /* test for primality */
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
