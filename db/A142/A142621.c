/*
 * A142621  Primes congruent to 28 mod 55.
 * https://oeis.org/A142621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 28 modulo 55 */
/* The function computes the n‑th prime p such that p ≡ 28 (mod 55). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    if (n == 0)                /* outside the offset – undefined, guard */
        return 0;

    idx_t count = 0;
    idx_t candidate = 28;      /* start just before the first admissible number */

    for (;;) {
        candidate += 55;       /* move to the next number congruent to 28 mod 55 */
        if (is_prime(candidate)) {
            count++;
            if (count == n)    /* found the n‑th prime of this congruence class */
                return (seq_t)candidate;
        }
    }
}
