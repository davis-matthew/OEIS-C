/*
 * A229876  Palindromic prime numbers == 2 (mod 9).
 * https://oeis.org/A229876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 994, u128 n <= 994,
 *            big any n
 * Helpers:   is_prime_big, pow10, reverse_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A229876:
 *  Palindromic prime numbers ≡ 2 (mod 9).
 *  The function enumerates palindromic numbers by length,
 *  tests the congruence and primality using the supplied
 *  utilities is_prime_big and pow10.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/reverse_part.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    idx_t found = 0;

    for (idx_t len = 1; ; ++len) {
        /* Even-length palindromes (except 2‑digit 11) are divisible by 11 */
        if ((len % 2 == 0) && len > 2)
            continue;

        idx_t half = (len + 1) / 2;           /* digits in the left half */

        seq_t start   = pow10((idx_t)(half - 1));        /* smallest left half */
        seq_t end     = pow10((idx_t)half) - (seq_t)1;   /* largest left half */

        for (seq_t left = start; left <= end; ++left) {
            seq_t rev = (len % 2 == 1)
                        ? reverse_part(left / (seq_t)10)
                        : reverse_part(left);

            seq_t pow_val = pow10((idx_t)(len - half));
            seq_t pal = left * pow_val + rev;

            if (pal <= (seq_t)1)
                continue;

            /* Check congruence to 2 modulo 9. */
            if (pal % (seq_t)9 != (seq_t)2)
                continue;

            if (is_prime_big(pal)) {
                ++found;
                if (found == n)
                    return pal;
            }
        }
    }
}
