/*
 * A142164  Primes congruent to 8 mod 39.
 * https://oeis.org/A142164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 254, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime congruent to 8 modulo 39 (offset 1). */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

static inline seq_t next_candidate(seq_t x)
{
    return x + (seq_t)39;
}

seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* not defined below offset */

    idx_t count = 0;
    seq_t cand   = (seq_t)8;      /* first number in the residue class 8 mod 39 */

    while (true) {
        if (is_prime_big(cand)) { /* true if cand is prime */
            count++;
            if (count == n) return cand;
        }
        cand = next_candidate(cand);
    }
}
