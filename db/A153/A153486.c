/*
 * A153486  a(n) = prime(n)^6 - 512.
 * https://oeis.org/A153486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n)^6 - 512
 * Range:     i8 none, i16 n <= 3, i32 n <= 11, i64 n <= 229, i128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is offset at 1, value for n=0 is unused */

    /* Fetch the nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute p^6 using a 128‑bit intermediate to avoid overflow. */
    __int128 prod = (__int128)p;
    for (int i = 1; i < 6; ++i) {
        prod *= p;   /* p^i+1 */
    }

    /* Subtract 512 and cast the 128‑bit result back to seq_t. */
    return (seq_t)(prod - 512);
}
