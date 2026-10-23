/*
 * A142739  Primes congruent to 12 mod 59.
 * https://oeis.org/A142739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 12 (mod 59) – search for the n‑th prime in this arithmetic progression. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1; n==0 is undefined but return 0 for safety */

    uint64_t candidate = 71;          /* first prime ≡ 12 (mod 59) */
    idx_t    found       = 0;

    while (found < n) {
        seq_t cand_seq = (seq_t)candidate;   /* convert to seq_t for the prime test */

        if (is_prime_big(cand_seq)) {        /* works for both small and big seq_t */
            ++found;
            if (found == n)
                return cand_seq;
        }

        candidate += 59;                    /* next member of the progression */
    }

    /* unreachable, but satisfies compiler */
    return (seq_t)candidate;
}
