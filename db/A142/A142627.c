/*
 * A142627  Primes congruent to 37 mod 55.
 * https://oeis.org/A142627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 37 modulo 55. The code
 *            iterates candidates 37 + 55*k and tests each for primality,
 *            caching results so that any order of calls is handled
 *            efficiently
 * Range:     u8 n <= 1, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 881 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cached primes */
    static size_t cap = 0;        /* current cache capacity */
    static size_t count = 0;      /* number of cached primes */
    static idx_t curr_k = 0;      /* K for candidate 37 + 55*K */

    /* Grow cache to start size if empty */
    if (cache == NULL) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        /* initial capacity is small; memory is freed by caller's lib */
    }

    /* Generate primes until we have the requested n-th term */
    while (count < n) {
        /* candidate prime candidate */
        idx_t cand = 37 + 55 * curr_k;
        /* test primality */
        if (is_prime_seq(cand)) {
            /* ensure space in cache */
            if (count == cap)
                grow_seq(&cache, &cap, count + 1);
            cache[count] = (seq_t)cand;
            ++count;
        }
        ++curr_k;
    }

    return cache[n - 1];
}
