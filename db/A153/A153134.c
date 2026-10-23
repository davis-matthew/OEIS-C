/*
 * A153134  Numbers k such that 6k - 7 is prime.
 * https://oeis.org/A153134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Computes the n‑th integer k such that 6*k - 7 is prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1 – return 0 */

    idx_t count = 0;
    idx_t k = 2;

    while (count < n)
    {
        seq_t val = (seq_t)6 * k - 7;  /* 6k - 7 in integer arithmetic */
        if (is_prime_big(val))
        {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }

    return 0; /* Should never be reached for n >= 1 */
}
