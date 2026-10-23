/*
 * A142649  Primes congruent to 19 mod 56.
 * https://oeis.org/A142649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 260, u32 n <= 261, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 19 mod 56 (search) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *qual_primes = NULL;
    static size_t qual_cnt = 0, qual_cap = 0;
    static uint64_t candidate = 19;  /* start from the first candidate */

    if (n == 0) return (seq_t)0;   /* offset is 1; return 0 for safety */

    /* Generate qualifying primes until we have at least n of them. */
    while (qual_cnt < n) {
        if (is_prime_big((seq_t)candidate)) {
            if (qual_cnt == qual_cap) {
                ensure_capacity(qual_cnt + 1, &qual_cap, (void **)&qual_primes, sizeof(seq_t));
            }
            qual_primes[qual_cnt++] = (seq_t)candidate;
        }
        candidate += 56;  /* next number that is 19 mod 56 */
    }

    return qual_primes[n - 1];
}
