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
 * Validated: full, pass, 1229 terms through n = 1229 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes < 10000 sorted lexicographically in decimal representation */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

/* Comparator for lexicographic ordering of decimal strings */
static int cmp_lex(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    char bufx[16], bufy[16];
    int lenx = 0, leny = 0;

    /* convert x to decimal string */
    uint64_t t = x;
    do {
        bufx[lenx++] = '0' + (t % 10);
        t /= 10;
    } while (t);
    for (int i = 0; i < lenx / 2; ++i) {
        char c = bufx[i];
        bufx[i] = bufx[lenx - 1 - i];
        bufx[lenx - 1 - i] = c;
    }

    /* convert y to decimal string */
    t = y;
    do {
        bufy[leny++] = '0' + (t % 10);
        t /= 10;
    } while (t);
    for (int i = 0; i < leny / 2; ++i) {
        char c = bufy[i];
        bufy[i] = bufy[leny - 1 - i];
        bufy[leny - 1 - i] = c;
    }

    int i = 0;
    while (i < lenx && i < leny) {
        if (bufx[i] != bufy[i])
            return bufx[i] < bufy[i] ? -1 : 1;
        ++i;
    }
    if (i == lenx && i == leny) return 0;       /* equal */
    if (i == lenx) return -1;                 /* x shorter → x < y */
    return 1;                                  /* y shorter → x > y */
}

/* Build and cache the sorted list of primes below 10000 */
static void init_cache(seq_t **cache, size_t *len) {
    uint64_t *primes = NULL;
    size_t pn = 0;

    /* obtain all primes ≤ 9999 */
    build_primes_upto(9999, &primes, &pn);

    /* sort lexicographically */
    qsort(primes, pn, sizeof(uint64_t), cmp_lex);

    *len = pn;
    *cache = mem_new(seq_t, pn);
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
