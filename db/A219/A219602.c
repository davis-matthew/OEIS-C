/*
 * A219602  Primes p such that p^2-2 is composite.
 * https://oeis.org/A219602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 30, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A219602: primes p such that p²‑2 is composite.
   Computed by incremental prime search (is_prime), testing p²‑2 with
   is_prime_big. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Ensure an array has room for at least 'needed' elements. */
static void *grow_to(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return *ptr;
}

/* Return the n‑th prime p with p²‑2 composite. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */

    /* Static state for out‑of‑order calls. */
    static seq_t *found = NULL;
    static size_t found_len = 0, found_cap = 0;
    static idx_t candidate = 2;     /* next integer to test */

    /* Keep generating until we have enough terms. */
    while (found_len < (size_t)n) {
        if (is_prime(candidate)) {
            /* test whether p²‑2 is composite */
            seq_t sq = (seq_t)candidate * (seq_t)candidate;
            seq_t test = sq - (seq_t)2;
            if (!is_prime_big(test)) {
                grow_to(found_len + 1, &found_cap, (void **)&found, sizeof(seq_t));
                found[found_len++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }

    return found[n - 1];
}
