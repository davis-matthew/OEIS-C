/*
 * A155213  a(n) = floor(prime(n)/9).
 * https://oeis.org/A155213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(prime(n)/9)
 * Range:     u8 n <= 54, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Get the nth prime (1‑based).  get_prime is 1‑based so we can call it directly. */
    uint32_t p = get_prime(n);
    return (seq_t)p / 9;          /* integer division is floor for unsigned */
}
