/*
 * A154521  The prime(n)-th digit of the concatenated positive nonprimes.
 * https://oeis.org/A154521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = A129808(prime(n))
 * Range:     u8 n <= 99, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digits_of, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/digits_of.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* nth prime */
    uint32_t p = get_prime(n);

    /* walk through nonprimes (1 is included) and accumulate digit count */
    idx_t total = 0;
    for (idx_t i = 1; ; ++i)
    {
        if (is_prime(i))
            continue;                     /* skip primes */

        int len = digits_of(i);            /* number of decimal digits of i */
        if (total + len >= p)
        {
            /* offset of desired digit in the decimal representation of i */
            idx_t offset = p - total - 1;   /* 0‑based from most significant digit */

            /* compute divisor = 10^(len - offset - 1) */
            seq_t divisor = 1;
            int power = len - (int)offset - 1;
            for (int k = 0; k < power; ++k)
                divisor *= 10;

            /* extract the digit */
            seq_t digit = ((((seq_t)i) / divisor) % 10);
            return digit;
        }
        total += len;
    }
}
