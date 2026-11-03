/*
 * A201781  Primes of the form 3*m^2 - 8.
 * https://oeis.org/A201781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime of the form 3*m^2 – 8 (m is a natural
 *            number, starting with 3). The function builds the sequence
 *            lazily, remembering already found values so that repeated calls
 *            are fast
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 3030, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 52 terms through n = 4870 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Sequence element type and index type are supplied by the build: */
extern seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* next candidate m to try (start at 3 for the first prime) */
    static idx_t m_next = 3;

    /* Extend cache until we have at least n elements */
    while (cache_len < n) {
        /* candidate value 3*m^2 – 8 */
        seq_t cand = (seq_t)m_next * (seq_t)m_next * 3 - 8;

        /* test primality of the candidate */
        if (cand >= 19 && is_prime((idx_t)cand)) {
            /* grow cache if necessary */
            if (cache_len >= cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);

            /* store the prime */
            cache[cache_len++] = cand;
        }
        ++m_next;
    }

    return cache[n - 1];
}
