/*
 * A155106  Numbers k such that both k and k+1 can be expressed as the sum of
 *          three distinct nonzero squares in 2 or more ways.
 * https://oeis.org/A155106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155106: Numbers k such that both k and k+1 can be expressed
   as the sum of three distinct non‑zero squares in two or more ways.
   This implementation incrementally computes all representations up
   to a dynamic bound B and builds the list of qualifying numbers. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Utility: integer square root (floor) ---------- */
static idx_t
isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* ---------- Global state for memoised sequence ---------- */
static seq_t *terms     = NULL;   /* list of qualifying numbers      */
static size_t terms_size   = 0;   /* number of terms stored          */
static size_t terms_cap    = 0;   /* capacity of terms array         */

/* ---------- Build terms for a given bound B ---------- */
static void
generate_terms_for_B(idx_t B)
{
    /* Allocate / zero count array (size B+1 to index B) */
    uint16_t *cnt = (uint16_t *)calloc((size_t)(B + 1), sizeof(uint16_t));
    if (!cnt) exit(1);

    /* Count representations using three distinct non‑zero squares */
    idx_t max_i = isqrt(B);
    for (idx_t i = 1; i <= max_i; ++i) {
        uint64_t sq_i = (uint64_t)i * i;
        for (idx_t j = i + 1; j <= max_i; ++j) {
            uint64_t sq_j = (uint64_t)j * j;
            uint64_t sum_ij = sq_i + sq_j;
            if (sum_ij > B) break;
            for (idx_t k = j + 1; k <= max_i; ++k) {
                uint64_t sum = sum_ij + (uint64_t)k * k;
                if (sum > B) break;
                ++cnt[(idx_t)sum];
            }
        }
    }

    /* Free previous terms array */
    free(terms);
    terms = NULL;
    terms_size = 0;
    terms_cap  = 0;

    /* Build the term list (k such that k and k+1 have ≥2 representations) */
    for (idx_t x = 1; x < B; ++x) {
        if (cnt[x] >= 2 && cnt[x + 1] >= 2) {
            if (terms_size == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 1024;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) { free(cnt); exit(1); }
                terms = tmp;
            }
            terms[terms_size++] = (seq_t)x; /* implicit conversion to seq_t */
        }
    }

    free(cnt);
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    /* Minimise recomputation by extending the bound only when needed. */
    static bool initialized = false;
    static idx_t current_B = 0;

    if (!initialized || terms_size < (size_t)n) {
        idx_t B = current_B ? current_B * 2 : 100000;
        while (true) {
            generate_terms_for_B(B);
            if (terms_size >= (size_t)n)
                break;
            B *= 2;
        }
        current_B = B;
        initialized = true;
    }

    /* Sequence is 1‑indexed */
    return terms[n - 1];
}
