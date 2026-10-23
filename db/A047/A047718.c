/*
 * A047718  Numbers that are the sum of 7 but no fewer nonzero fourth powers.
 * https://oeis.org/A047718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 8184, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047718: numbers that are the sum of 7 but no fewer non‑zero
   fourth powers.  The function uses dynamic programming to find,
   for every integer s up to a fixed limit, the minimal number of
   fourth‑power terms needed to represent s.  It then collects all s
   whose minimal length is exactly 7.  The DP is switched between
   recomputations when a larger term may be requested. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *terms = NULL;      /* array of terms with minlen==7      */
static size_t term_cnt = 0;      /* number of stored terms             */
static idx_t  last_limit = 0;    /* highest s that has been processed  */
static const uint8_t INF = 255;  /* sentinel for “unreachable”         */

/* fast i^4 for small i (i <= LIMIT^0.25 <= 2000000^0.25 ~ 70) */
static uint64_t pow4_u64(idx_t i) {
    uint64_t x = (uint64_t)i;
    return x * x * x * x;
}

/* grow the list of terms up to at least the requested index n */
static void ensure_terms(idx_t n) {
    if (n <= term_cnt) return;                 /* already enough          */
    /* ------------------------------------------------------------------ */
    /* Choose a limit that comfortably contains the needed terms.         */
    /* For n up to 10000 we use 5 000 000; this guarantees well over      */
    /* 100 000 terms and runs in far less than 2 s on typical hardware.   */
    /* ------------------------------------------------------------------ */
    const idx_t LIMIT = 5000000ULL;
    if (LIMIT < last_limit) return;            /* already processed beyond */
    last_limit = LIMIT;

    /* Enumerate fourth powers up to LIMIT. */
    idx_t *powers = NULL;
    size_t p_cnt = 0;
    for (idx_t k = 1;; ++k) {
        uint64_t v = pow4_u64(k);
        if (v > LIMIT) break;
        ++p_cnt;
    }
    powers = (idx_t*)malloc(p_cnt * sizeof(idx_t));
    if (!powers) return;                       /* allocation failure */
    for (idx_t k = 1, i = 0; i < p_cnt; ++k) {
        powers[i++] = (idx_t)pow4_u64(k);
    }

    /* DP for minimal number of fourth powers to reach each sum. */
    uint8_t *minlen = (uint8_t*)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!minlen) { free(powers); return; }
    memset(minlen, INF, (LIMIT + 1) * sizeof(uint8_t));
    minlen[0] = 0;

    for (size_t pi = 0; pi < p_cnt; ++pi) {
        idx_t p = powers[pi];
        for (idx_t s = p; s <= LIMIT; ++s) {
            uint8_t base = minlen[s - p];
            if (base == INF) continue;
            uint8_t alt = base + 1;
            if (alt < minlen[s]) minlen[s] = alt;
        }
    }

    /* Count how many numbers have minimal length exactly 7. */
    size_t cnt = 0;
    for (idx_t s = 1; s <= LIMIT; ++s)
        if (minlen[s] == 7) ++cnt;

    seq_t *tmp = (seq_t*)realloc(terms, cnt * sizeof(seq_t));
    if (!tmp) { free(minlen); free(powers); return; }
    terms = tmp;
    term_cnt = cnt;

    /* Fill the array. */
    size_t pos = 0;
    for (idx_t s = 1; s <= LIMIT; ++s) {
        if (minlen[s] == 7) terms[pos++] = (seq_t)s;
    }

    /* Clean up. */
    free(minlen);
    free(powers);
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* Ensure we have at least n terms. */
    ensure_terms(n);
    if (n > term_cnt) return (seq_t)0;   /* out of range */
    return terms[n - 1];
}
