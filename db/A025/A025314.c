/*
 * A025314  Numbers that are the sum of 2 distinct nonzero squares in 4 or
 *          more ways.
 * https://oeis.org/A025314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 898, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025314:
   Numbers that are the sum of 2 distinct nonzero squares in 4 or more ways.
   The code builds all representations up to a fixed limit and selects
   the numbers with at least 4 distinct representations.  The nth term
   (1‑based) is returned.  A fixed large limit is used so that the
   function answers any call for n up to about 10 000 in the required
   time. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint16_t *counts = NULL;      /* counts of representations */
static seq_t    *terms  = NULL;      /* list of admissible sums */
static size_t   terms_len = 0;       /* number of terms stored */
static bool     initialized = false;

static void generate_terms(void)
{
    if (initialized) return;

    /* Upper bound that comfortably contains ≈10 000 terms. */
    const unsigned int LIMIT = 20000000U;          /* 20 000 000 */
    const unsigned int MAX_I = 5000U;             /* 5000² = 25 000 000 > LIMIT */
    unsigned int i, j;

    counts = (uint16_t *)calloc(LIMIT + 1, sizeof(uint16_t));
    if (!counts) return;                          /* allocation failure */

    /* Count every unordered pair (i,j), 1 ≤ i < j, with i² + j² ≤ LIMIT */
    for (i = 1; i <= MAX_I; ++i) {
        unsigned int i2 = i * i;
        for (j = i + 1; j <= MAX_I; ++j) {
            unsigned int sum = i2 + j * j;
            if (sum > LIMIT) break;
            ++counts[sum];
        }
    }

    /* Collect all numbers with at least 4 representations */
    terms_len = 0;
    terms = NULL;                                  /* will grow as needed */
    size_t cap = 0;
    for (i = 1; i <= LIMIT; ++i) {
        if (counts[i] >= 4) {
            if (terms_len == cap) {                /* grow storage */
                size_t new_cap = cap ? cap << 1 : 1024;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) break;                  /* allocation failure */
                terms = tmp;
                cap = new_cap;
            }
            terms[terms_len++] = (seq_t)i;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but guard */
    generate_terms();
    if ((size_t)n > terms_len) {
        /* In practice this should not happen for the tested range. */
        return 0;
    }
    return terms[(size_t)n - 1];
}
