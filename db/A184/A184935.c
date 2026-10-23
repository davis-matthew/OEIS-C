/*
 * A184935  Primes of the form k^2 + prime(k).
 * https://oeis.org/A184935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 32, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184935: primes of the form k^2 + prime(k).  The function
   searches for successive k, tests primality of k^2 + prime(k) using
   the provided helpers, and stores found primes for future calls. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t k_next = 1;          /* next k to test */

    /* Grow the cache until we have at least n elements. */
    while (seq_len < (size_t)n) {
        /* compute candidate: k^2 + prime(k) */
        seq_t k_sq = (seq_t)k_next * (seq_t)k_next;
        uint32_t pk = get_prime(k_next);
        seq_t val = k_sq + (seq_t)pk;

        /* test primality */
        if (is_prime_big(val)) {
            /* ensure capacity for the new element */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) {
                    abort();
                }
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = val;
        }
        ++k_next;
    }
    return seq[(size_t)n - 1];
}
