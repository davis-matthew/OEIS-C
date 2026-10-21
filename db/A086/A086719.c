/*
 * A086719  Numbers n == 1, 2, 5 or 6 (mod 8) such that there is only one
 *          solution to i^2+j^2+k^2=n, i >= j >= k >= 0.
 * https://oeis.org/A086719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A086719 – numbers n ≡ 1, 2, 5, 6 (mod 8) that have exactly one
    representation n = i² + j² + k² with i ≥ j ≥ k ≥ 0.
    The function a(n) returns the n‑th such number (a(1)=1, a(2)=2, …).
    Computation is done by exhaustive search over all triples with
    i² + j² + k² ≤ current limit.  The search is extended until
    at least n terms are found.  The implementation handles
    arbitrary-sized seq_t and idx_t as described in the build system.  */

#include <stdlib.h>
#include <stdint.h>

/* --------------  Static state  ----------------- */
static seq_t *terms = NULL;   /* stored qualifying numbers */
static size_t terms_count = 0;

/* --------------  Helper: full recomputation  ----------------- */
static void recompute_terms(idx_t limit)
{
    /* Allocate a byte array: 0 = none, 1 = one, 2 = more */
    uint8_t *count = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!count) { free(terms); terms = NULL; terms_count = 0; return; }

    /* Enumerate all triples with i ≥ j ≥ k ≥ 0 */
    for (idx_t i = 0; i * i <= limit; ++i) {
        idx_t i2 = i * i;
        for (idx_t j = 0; j <= i && i2 + j * j <= limit; ++j) {
            idx_t ij = i2 + j * j;
            for (idx_t k = 0; k <= j && ij + k * k <= limit; ++k) {
                idx_t s = ij + k * k;
                if (s == 0) continue;          /* skip zero */
                if (count[s] < 2) ++count[s];   /* count up to 2 */
            }
        }
    }

    /* Collect qualifying numbers */
    seq_t *new_terms = NULL;
    size_t new_len = 0, new_cap = 0;

    for (idx_t s = 1; s <= limit; ++s) {
        if (count[s] == 1 && ((s & 7ULL) == 1ULL || (s & 7ULL) == 2ULL ||
                              (s & 7ULL) == 5ULL || (s & 7ULL) == 6ULL)) {
            if (new_len == new_cap) {
                new_cap = new_cap ? new_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(new_terms, new_cap * sizeof(seq_t));
                if (!tmp) { free(count); free(new_terms); free(terms);
                            terms = NULL; terms_count = 0; return; }
                new_terms = tmp;
            }
            new_terms[new_len++] = (seq_t)s;
        }
    }

    free(count);
    free(terms);   /* discard previous list */
    terms = new_terms;
    terms_count = new_len;
}

/* --------------  Ensure at least n terms  ----------------- */
static void ensure_terms(idx_t n)
{
    if (n <= terms_count) return;           /* already enough */

    idx_t limit = 128;                       /* initial search bound */
    while (terms_count < n) {
        recompute_terms(limit);
        if (terms_count < n) limit *= 2;
    }
}

/* --------------  Public accessor  ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑based; 0 is undefined */
    ensure_terms(n);
    return terms[n - 1];
}
