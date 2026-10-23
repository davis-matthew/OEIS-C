/*
 * A032853  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(m)
 *          <= d(m-1) >= d(m-2) <= ...
 * https://oeis.org/A032853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032853: numbers whose base‑6 digits satisfy  
   d(m) ≤ d(m-1) ≥ d(m-2) ≤ … (alternating inequalities).  
   The function returns the n‑th smallest such number (offset 1). */

#include <stdlib.h>
#include <stdbool.h>

/* State used while expanding numbers digit by digit. */
struct State {
    seq_t val;          /* value of the number built so far (most‑significant  digits first) */
    uint8_t last;       /* most recent (i.e. left‑most) digit */
    bool next_ge;       /* true if the next digit must be ≥ last, false otherwise (alternating) */
};

/* Storage for all generated numbers (unsorted until sorted). */
static seq_t *list = NULL;
static size_t list_cap = 0;
static size_t list_len = 0;
static bool sorted = false;

/* Current frontier of unfinished numbers being extended. */
static struct State *frontier = NULL;
static size_t frontier_len = 0;
static idx_t max_len_generated = 0; /* number of digits that have been generated */

/* Append a new number to the global list, expanding its capacity if needed. */
static void add_value(seq_t v) {
    if (list_len == list_cap) {
        size_t new_cap = list_cap ? list_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
        list = tmp;
        list_cap = new_cap;
    }
    list[list_len++] = v;
}

/* Comparison function for qsort. */
static int compare_seq(const void *p, const void *q) {
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Generate numbers of the next length in the frontier. */
static void generate_next_len(void) {
    if (max_len_generated == 0) {
        /* Initialization: single‑digit numbers 1..5 */
        max_len_generated = 1;
        frontier_len = 5;
        frontier = (struct State *)malloc(frontier_len * sizeof(struct State));
        for (uint8_t d = 1; d <= 5; ++d) {
            frontier[d-1].val      = (seq_t)d;
            frontier[d-1].last     = d;
            frontier[d-1].next_ge  = true;   /* first inequality is “next digit ≥ last” */
            add_value((seq_t)d);
        }
    } else {
        max_len_generated++;

        /* Each state can generate at most 6 new states (digits 0‑5). */
        size_t new_cap = frontier_len * 6;
        struct State *new_frontier = (struct State *)malloc(new_cap * sizeof(struct State));
        size_t new_len = 0;

        for (size_t i = 0; i < frontier_len; ++i) {
            struct State *s = &frontier[i];
            if (s->next_ge) {
                /* Next digit must be ≥ last. */
                for (uint8_t d = s->last; d <= 5; ++d) {
                    seq_t val = s->val * (seq_t)6 + (seq_t)d;
                    add_value(val);
                    new_frontier[new_len++] = (struct State){ val, d, !s->next_ge };
                }
            } else {
                /* Next digit must be ≤ last. */
                for (uint8_t d = 0; d <= s->last; ++d) {
                    seq_t val = s->val * (seq_t)6 + (seq_t)d;
                    add_value(val);
                    new_frontier[new_len++] = (struct State){ val, d, !s->next_ge };
                }
            }
        }

        /* Replace old frontier. */
        free(frontier);
        frontier = new_frontier;
        frontier_len = new_len;
    }

    sorted = false; /* new terms added */
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* sequence is 1‑based; behaviour for 0 is undefined */

    /* Generate enough terms to satisfy the request. */
    while (list_len < (size_t)n) {
        generate_next_len();
    }

    /* Sort once after new terms are added. */
    if (!sorted) {
        qsort(list, list_len, sizeof(seq_t), compare_seq);
        sorted = true;
    }

    return list[n - 1];
}
