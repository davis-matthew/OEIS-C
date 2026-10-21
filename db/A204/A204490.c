/*
 * A204490  Number of (n+2)X8 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly one way, and new values 0..1 introduced in row
 *          major order.
 * https://oeis.org/A204490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 19, u64 n <= 51, u128 n <= 112,
 *            big any n
 * Helpers:   good_trip
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A204490.
   The first 33 terms are obtained by a dynamic‑programming
   enumeration of 3×8 binary arrays.  For n > 33 the sequence
   satisfies the linear recurrence
       a(n)=a(n-1)+a(n-2)+9a(n-3)-5a(n-4)-10a(n-5)-7a(n-6)
            -28a(n-7)+16a(n-8)-9a(n-9)+40a(n-10)+5a(n-11)
            +2a(n-12)+16a(n-13)-19a(n-14)+10a(n-15)-20a(n-16)
            +3a(n-17)-2a(n-18)-5a(n-19)+4a(n-20)-2a(n-21)+a(n-22).
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_trip.h"

/* --------- DP constants --------------------------------------- */
#define STATES      65536          /* 256 × 256 row pairs */
#define MAX_INIT    33              /* number of DP terms pre‑computed */
#define COEF_COUNT  22

/* --------- recurrence coefficients ----------------------------- */
static const int COEFF[COEF_COUNT] = {
    1,  1,   9, -5, -10, -7, -28, 16, -9, 40,
    5,  2,  16, -19, 10, -20,  3, -2, -5,  4,
   -2,  1
};

/* --------- adjacency edge list --------------------------------- */
typedef struct {
    uint32_t *data;
    size_t len;
    size_t cap;
} EdgeList;

static EdgeList adjacency[STATES];

/* --------- dynamic programming workspace ---------------------- */
static seq_t *dp_prev = NULL;
static seq_t *dp_cur  = NULL;

/* --------- pre‑computed terms --------------------------------- */
static seq_t seeds[MAX_INIT + 1];   /* seeds[1] … seeds[33] */

/* --------- cache for arbitrary‑order calls -------------------- */
static seq_t *cache      = NULL;
static size_t cache_size = 0;

/* --------- initialisation flag -------------------------------- */
static bool initialized = false;

/* --------- push an adjacency edge ----------------------------- */
static void push_edge(size_t src, uint32_t dest) {
    EdgeList *el = &adjacency[src];
    if (el->data == NULL) {
        el->cap = 4;
        el->data = (uint32_t *)malloc(el->cap * sizeof(uint32_t));
    } else if (el->len == el->cap) {
        el->cap <<= 1;
        el->data = (uint32_t *)realloc(el->data, el->cap * sizeof(uint32_t));
    }
    el->data[el->len++] = dest;
}

/* --------- one‑time initialization -------------------------------- */
static void init_once(void) {
    /* allocate DP buffers */
    dp_prev = (seq_t *)malloc(sizeof(seq_t) * STATES);
    dp_cur  = (seq_t *)malloc(sizeof(seq_t) * STATES);

    /* each 2‑row pair can start the array */
    for (size_t i = 0; i < STATES; ++i) {
        dp_prev[i] = 1;
        dp_cur[i]  = 0;
    }

    /* build adjacency graph: from (r1,r2) to (r2,r3) */
    for (int r1 = 0; r1 < 256; ++r1) {
        for (int r2 = 0; r2 < 256; ++r2) {
            size_t src = ((size_t)r1 << 8) | (size_t)r2;
            for (int r3 = 0; r3 < 256; ++r3) {
                if (good_trip(r1, r2, r3)) {
                    push_edge(src, (uint32_t)(((size_t)r2 << 8) | (size_t)r3));
                }
            }
        }
    }

    /* compute the first MAX_INIT terms */
    for (size_t step = 1; step <= MAX_INIT; ++step) {
        /* reset current buffer */
        for (size_t i = 0; i < STATES; ++i) dp_cur[i] = 0;

        /* transition */
        for (size_t src = 0; src < STATES; ++src) {
            seq_t val = dp_prev[src];
            if (!val) continue;
            EdgeList *el = &adjacency[src];
            for (size_t j = 0; j < el->len; ++j)
                dp_cur[el->data[j]] += val;
        }

        /* total number of arrays for this length, divided by two
           to account for complement symmetry */
        seq_t sum = 0;
        for (size_t i = 0; i < STATES; ++i) sum += dp_cur[i];
        seeds[step] = sum >> 1;   /* right shift = divide by 2 */

        /* swap buffers for next step */
        seq_t *tmp = dp_prev; dp_prev = dp_cur; dp_cur = tmp;
    }

    /* initialise cache with the pre‑computed values */
    cache_size = MAX_INIT;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
    for (size_t i = 1; i <= cache_size; ++i)      /* cache[0] = a(1) */
        cache[i-1] = seeds[i];

    initialized = true;
}

/* --------- sequence implementation -------------------------------- */
seq_t a(idx_t n) {
    if (!initialized) init_once();
    if (n < 1) return 0;  /* undefined, but harmless */

    /* return cached value if already computed */
    if ((idx_t)cache_size >= n) return cache[n-1];

    /* extend cache up to n using the recurrence */
    size_t old_size = cache_size;
    cache_size = (size_t)n;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);

    for (size_t k = old_size + 1; k <= n; ++k) {
        seq_t pos = 0, neg = 0;
        for (int i = 1; i <= COEF_COUNT; ++i) {
            int c = COEFF[i-1];
            seq_t term = cache[(size_t)(k - i) - 1];
            if (c > 0) pos += term * (seq_t)c;
            else       neg += term * (seq_t)(-c);
        }
        cache[k-1] = pos - neg;
    }
    return cache[n-1];
}
