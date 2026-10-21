/*
 * A228593  Products of 2 successive primorials.
 * https://oeis.org/A228593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = primorial(n-1) * primorial(n) Recurrence: a(1) = 2 a(n) =
 *            a(n-1) * prime(n-1) * prime(n) for n ≥ 2 (primes are enumerated
 *            from 1: prime(1) = 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"

/* ensure the cache can hold at least (n+1) elements (index 1..n) */
static void ensure_capacity(idx_t n, seq_t **cache, size_t *cap) {
    size_t needed = (size_t)n + 1;
    if (*cap >= needed) return;
    /* grow (or allocate new) */
    if (*cache == NULL) {
        *cache = (seq_t*)malloc(needed * sizeof(seq_t));
    } else {
        *cache = (seq_t*)realloc(*cache, needed * sizeof(seq_t));
    }
    if (*cache == NULL) abort();      /* memory exhaustion */
    *cap = needed;
}

seq_t a(idx_t n) {
    /* a(1) is the first term, behavior below 1 does not matter */
    if (n == 0) return 0;

    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_n = 0;

    /* make room for index n */
    ensure_capacity(n, &cache, &cap);

    /* initialise first term if cache is empty */
    if (max_n == 0) {
        cache[1] = (seq_t)2;          /* primorial(0)*primorial(1) = 1*2 */
        max_n   = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t prev = cache[i-1];
        /* fetch successive primes */
        uint32_t p1 = get_prime(i-1);  /* prime(i-1) */
        uint32_t p2 = get_prime(i);    /* prime(i) */
        /* multiply */
        seq_t tmp = seq_mul((seq_t)p1, (seq_t)p2);
        cache[i] = seq_mul(prev, tmp);
    }
    max_n = n;

    return cache[n];
}
