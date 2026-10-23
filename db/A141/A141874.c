/*
 * A141874  Primes congruent to 9 mod 19.
 * https://oeis.org/A141874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 9 (mod 19)
 * Range:     u8 n <= 2, u16 n <= 371, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for n < 1 */

    /* Static storage for memoisation */
    static seq_t *sequence = NULL;            /* array of found primes */
    static size_t seq_cap  = 0;               /* capacity of the array   */
    static size_t seq_len  = 0;               /* number of primes stored */
    static idx_t last_checked = 1;            /* integer last tested */

    /* Ensure array has room for at least one more element */
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                   /* allocation failure */
        sequence = tmp;
        seq_cap   = new_cap;
    }

    /* Generate primes until we have the required count */
    while (seq_len < n) {
        ++last_checked;                      /* move to next integer */

        if (!is_prime(last_checked))
            continue;                         /* skip non‑primes */

        if (last_checked % 19 == 9) {
            /* Store the prime */
            if (seq_len == seq_cap) {         /* just in case growth happened */
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(sequence,
                                               new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                sequence = tmp;
                seq_cap   = new_cap;
            }
            sequence[seq_len++] = (seq_t)last_checked;
        }
    }

    return sequence[n - 1];
}
