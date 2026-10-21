/*
 * A210759  Primes less than 10000 sorted lexicographically in decimal
 *          representation.
 * https://oeis.org/A210759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1229, u32 n <= 1229, u64 n <= 1229,
 *            u128 n <= 1229, big any n
 * Helpers:   build_primes_upto, cmp_lex
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes < 10000 sorted lexicographically in decimal representation */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/cmp_lex.h"

/* Build and cache the sorted list of primes below 10000 */
static void init_cache(seq_t **cache, size_t *len) {
    uint64_t *primes = NULL;
    size_t pn = 0;

    /* obtain all primes ≤ 9999 */
    build_primes_upto(9999, &primes, &pn);

    /* sort lexicographically */
    qsort(primes, pn, sizeof(uint64_t), cmp_lex);

    *len = pn;
    *cache = (seq_t *)malloc(pn * sizeof(seq_t));
    for (size_t i = 0; i < pn; ++i)
        (*cache)[i] = (seq_t)primes[i];

    free(primes);
}

/* OEIS sequence A210759: primes < 10000 sorted lexicographically */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (!initialized) {
        init_cache(&cache, &cache_len);
        initialized = true;
    }

    if ((size_t)(n - 1) >= cache_len)
        return 0;                           /* out of range; 1229 terms total */

    return cache[n - 1];
}
