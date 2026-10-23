/*
 * A031345  Write primes in base 10 but interpret as if in base 13.
 * https://oeis.org/A031345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = base13( prime(n) )
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Convert the n‑th prime (in decimal) to base‑13 interpretation. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Public sequence function.
   n must be >= 1; the result for n<1 is defined as 0. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n<1 */

    /* Find the n‑th prime by simple trial division. */
    idx_t count = 0;
    idx_t num   = 1;                     /* start below the first prime */
    while (count < n) {
        num++;                           /* candidate prime */
        if (is_prime(num)) {
            count++;                     /* found another prime */
        }
    }

    /* Interpret the decimal digits of num as base‑13 digits. */
    seq_t value = 0;                     /* result in base‑13 value */
    seq_t mult  = 1;                     /* 13^k multiplier */
    idx_t work  = num;                   /* copy to avoid modifying num */
    while (work > 0) {
        seq_t digit = (seq_t)(work % 10);       /* decimal digit */
        value += digit * mult;                 /* add digit * 13^k */
        mult *= 13;                            /* next power of 13   */
        work /= 10;
    }
    return value;
}
