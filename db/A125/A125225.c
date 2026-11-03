/*
 * A125225  Numbers n such that n-1 can be represented as a sum of a subset of
 *          divisors of n.
 * https://oeis.org/A125225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 990 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer n (n≥1) such that n-1 can be expressed as a sum
   of a subset of the proper divisors of n.  The function returns the
   value of the sequence at index n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether n-1 can be represented as a subset sum of
   proper divisors of n.  n is a 64‑bit unsigned integer. */
static bool has_subset_sum_uint64(uint64_t n)
{
    if (n == 1ULL) return true;          /* target 0 = empty sum */

    /* Build list of proper divisors of n. */
    size_t cap = 16;
    uint64_t *divs = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!divs) return false;
    size_t divcnt = 0;

    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            uint64_t d1 = i, d2 = n / i;
            if (d1 != n) {
                if (divcnt == cap) {
                    size_t newcap = cap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(divs, newcap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return false; }
                    divs = tmp; cap = newcap;
                }
                divs[divcnt++] = d1;
            }
            if (d2 != d1 && d2 != n) {
                if (divcnt == cap) {
                    size_t newcap = cap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(divs, newcap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return false; }
                    divs = tmp; cap = newcap;
                }
                divs[divcnt++] = d2;
            }
        }
    }

    uint64_t target = n - 1ULL;
    /* target is >0 here */
    size_t wordlen = (target >> 6) + 1;   /* number of 64‑bit words needed */

    uint64_t *bits = (uint64_t*)calloc(wordlen, sizeof(uint64_t));
    if (!bits) { free(divs); return false; }
    bits[0] = 1ULL;  /* sum 0 achievable */

    for (size_t di = 0; di < divcnt; ++di) {
        uint64_t d = divs[di];
        size_t shift_words = d >> 6;      /* d / 64 */
        size_t shift_bits  = d & 63U;      /* d % 64 */

        /* Shift bits left by d and OR into bits */
        if (shift_words >= wordlen) continue; /* shift beyond target */

        /* 32‑bit temporary storage for shifted word */
        for (size_t i = wordlen; i-- > 0; ) {
            uint64_t val = 0ULL;
            if (i >= shift_words) {
                val = bits[i - shift_words] << shift_bits;
                if (shift_bits && i > shift_words)
                    val |= bits[i - shift_words - 1] >> (64U - shift_bits);
            }
            bits[i] |= val;
        }

        if (bits[target >> 6] & (1ULL << (target & 63U))) {
            free(divs);
            free(bits);
            return true;
        }
    }

    free(divs);
    free(bits);
    return false;
}

/* Static cache of found sequence values */
static seq_t *found      = NULL;
static size_t foundsize  = 0U;   /* number of elements stored */
static size_t foundcap   = 0U;   /* allocated capacity */

/* Next candidate integer to test (starting from 1). */
static uint64_t next_candidate = 1U;

/* Sequence function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but avoid crash */

    size_t nidx = (size_t)n;       /* index in 0‑based array */

    /* Generate terms until requested index is reached. */
    while (foundsize < nidx) {
        uint64_t cand = next_candidate++;
        if (has_subset_sum_uint64(cand)) {
            /* Ensure capacity for found array. */
            if (foundsize >= foundcap) {
                size_t newcap = foundcap ? foundcap * 2ULL : 16ULL;
                seq_t *tmp = mem_resize(seq_t, found, newcap);
                if (!tmp) break;        /* allocation failure */
                found = tmp;
                foundcap = newcap;
            }
            found[foundsize++] = (seq_t)cand;
        }
    }

    /* Return cached value. */
    return found[nidx - 1U];
}
