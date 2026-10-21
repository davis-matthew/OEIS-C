/*
 * A210761  Primes less than 1000000 sorted lexicographically in decimal
 *          representation.
 * https://oeis.org/A210761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the n‑th prime < 1,000,000 sorted lexicographically in
 *            decimal representation
 * Range:     u8 none, u16 none, u32 n <= 9111, u64 n <= 9111, u128 n <= 9111,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"

static seq_t *lex_primes = NULL;   /* sorted primes */
static size_t   lex_count  = 0;
static bool     init_done  = false;

/* Comparator for lexicographic order of decimal strings */
static int cmp_lex(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    char sx[32], sy[32];

    /* All primes are < 1,000,000, so fit in unsigned long long. */
    snprintf(sx, sizeof sx, "%llu", (unsigned long long)x);
    snprintf(sy, sizeof sy, "%llu", (unsigned long long)y);

    return strcmp(sx, sy);
}

/* Return the n-th term of A210761.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (!init_done) {
        /* Build all primes < 1,000,000 */
        uint64_t *temp = NULL;
        size_t    cnt  = 0;
        build_primes_upto(999999, &temp, &cnt);

        lex_primes = (seq_t *)malloc(cnt * sizeof(seq_t));
        for (size_t i = 0; i < cnt; ++i)
            lex_primes[i] = (seq_t)temp[i];

        free(temp);
        lex_count = cnt;

        qsort(lex_primes, lex_count, sizeof(seq_t), cmp_lex);
        init_done = true;
    }

    if (n < 1 || (idx_t)n > lex_count)
        return (seq_t)0;          /* out of range – return 0 */

    return lex_primes[(size_t)(n - 1)];
}
