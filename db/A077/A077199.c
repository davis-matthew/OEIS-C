/*
 * A077199  Smallest k such that both k and k+n are squarefree.
 * https://oeis.org/A077199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_sf_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest k > 1 such that both k and k+n are squarefree. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_sf_trial.h"

#define MAXL 2000000      /* maximum number for the pre‑computed table */

/* static cache of squarefree flags */
static unsigned char *sqfree = NULL;      /* 1 = squarefree, 0 = not */
static size_t sqfree_size = 0;

/* list of primes up to sqrt(MAXL) */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* forward declaration */
static void init(void);

/* ----- Helper to initialise cache ------------------------------------ */
static void init(void)
{
    if (sqfree != NULL) return;          /* already initialised */

    /* -------- build squarefree table --------------------------------- */
    sqfree_size = MAXL + 1;
    sqfree = (unsigned char *)malloc(sqfree_size);
    if (!sqfree) return;                 /* allocation failed – leave NULL */

    for (size_t i = 0; i < sqfree_size; ++i) sqfree[i] = 1;
    sqfree[0] = 0;                       /* 0 is not considered squarefree */

    /* -------- find primes up to sqrt(MAXL) --------------------------- */
    size_t lim = (size_t)floor(sqrt((double)MAXL)) + 1;
    unsigned char *small = (unsigned char *)calloc(lim + 1, 1);
    if (!small) return;

    for (size_t i = 2; i * i <= lim; ++i)
        if (!small[i])
            for (size_t j = i * i; j <= lim; j += i) small[j] = 1;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (!small[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) { free(small); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (!small[i]) prime_list[idx++] = (uint32_t)i;
    prime_count = cnt;
    free(small);

    /* -------- mark non‑squarefree numbers ----------------------------- */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint32_t p = prime_list[pi];
        uint32_t sq = p * p;
        for (size_t m = sq; m <= MAXL; m += sq) sqfree[m] = 0;
    }
}

/* ----- the required sequence function -------------------------------- */
seq_t a(idx_t n)
{
    init();                               /* ensure cache is ready */

    /* n is 1‑based (first term corresponds to n=1) */
    if (n == 0) return 0;                 /* not expected */

    size_t nn = (size_t)n;

    /* search within the pre‑computed table if possible */
    if (sqfree && nn < sqfree_size) {
        size_t limit_k = sqfree_size - nn;
        if (limit_k < 2) limit_k = 1;      /* ensure we start from k=2 */
        for (size_t k = 2; k <= limit_k; ++k)
            if (sqfree[k] && sqfree[k + nn])
                return (seq_t)k;
    }

    /* fall back to trial division if table insufficient */
    size_t k = 2;
    for (;;) {
        if (is_sf_trial(k) && is_sf_trial(k + nn))
            return (seq_t)k;
        ++k;
    }
}
