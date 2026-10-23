/*
 * A037047  Starting at n, "say what you see"; sequence gives number of primes
 *          obtained before first composite number appears.
 * https://oeis.org/A037047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of primes obtained in the * look‑and‑say
 *            sequence starting at n until the first composite is seen. * The
 *            starting n itself is not counted
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 50, u64 n <= 9670,
 *            u128 n <= 9670, big any n
 * Helpers:   is_prime_big, next_look_and_say
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_look_and_say.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;          /* start value (n itself is not counted) */
    seq_t primes_before_composite = 0;

    while (1) {
        seq_t nxt = next_look_and_say(cur);

        if (!is_prime_big(nxt))
            break;                     /* first composite encountered */

        primes_before_composite++;    /* prime found */
        cur = nxt;                    /* continue the chain */
    }

    return primes_before_composite;
}
