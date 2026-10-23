/*
 * A032707  Numbers k such that k prefixed by '7' and followed by '1' is
 *          prime.
 * https://oeis.org/A032707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k such that the decimal number formed by
 *            writing '7', then k, then '1' is prime. That is, N =
 *            7*10^(digits(k)+1) + k*10 + 1 is prime
 * Range:     u8 none, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Any n < 1 is outside the definition, just return 0. */
    if (n == 0) return (seq_t)0;

    idx_t count = 0;   /* how many suitable k's we have found */
    idx_t k = 0;       /* current candidate k */

    for (;; ++k) {
        /* Compute number of decimal digits of k (k==0 has 1 digit). */
        idx_t d = 0;
        idx_t temp = k;
        if (temp == 0) {
            d = 1;
        } else {
            while (temp) {
                ++d;
                temp /= 10;
            }
        }

        /* Build the number 7k1 in decimal. */
        seq_t N = (seq_t)7 * pow10(d + 1) + (seq_t)k * (seq_t)10 + (seq_t)1;

        /* Test primality. */
        if (isPrimeGeneric(N)) {
            ++count;                 /* one more suitable k found */
            if (count == n)          /* nth suitable k reached */
                return (seq_t)k;     /* return k itself as a(n) */
        }
    }
}
