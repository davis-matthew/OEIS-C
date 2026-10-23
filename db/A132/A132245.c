/*
 * A132245  Twin primes congruent to {1, 11, 13, 29} mod 30.
 * https://oeis.org/A132245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth prime belonging to a twin prime pair and
 *            congruent to 1, 11, 13, or 29 modulo 30 (offset 1)
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                /* Number of qualifying primes found */
    for (uint64_t p = 5; ; ++p) {    /* Scan all integers greater than 5 */
        uint8_t r = p % 30;
        if (r != 1 && r != 11 && r != 13 && r != 29) /* Only allowed residues */
            continue;

        seq_t prime = (seq_t)p;
        if (!is_prime_big(prime))            /* Must itself be prime */
            continue;

        /* Check if it belongs to a twin‑prime pair */
        seq_t prev = prime - (seq_t)2;
        seq_t next = prime + (seq_t)2;
        if (!is_prime_big(prev) && !is_prime_big(next))
            continue;            /* Not part of a twin pair */

        ++count;                 /* Found one qualifying prime */
        if (count == n)          /* Desired index reached */
            return prime;
    }
    /* Never reached */
}
