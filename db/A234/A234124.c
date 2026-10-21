/*
 * A234124  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having the absolute values of all six edge and diagonal
 *          differences no larger than 1.
 * https://oeis.org/A234124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i,j} (M^n)_{i,j} for the adjacency matrix M of all
 *            19 pairs (x,y) of 0…6 with |x−y|≤1 and edges defined by
 *            |x−x'|,|y−y'|,|x−y'|,|y−x'|≤1. Initial terms: a(1)=91, a(2)=469,
 *            …
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   uabs_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/uabs_diff.h"

/* constants */
#define NUM_STATES 19
#define MAX_VAL 7U

/* static storage for the graph and DP */
static uint8_t adj[NUM_STATES][NUM_STATES] = {0};
static uint8_t sx[NUM_STATES] = {0};
static uint8_t sy[NUM_STATES] = {0};

static seq_t *a_values = NULL;          /* a(1),a(2),… */
static size_t a_capacity = 0;
static seq_t *cur_vec = NULL;           /* number of paths ending at each state */
static seq_t *next_vec = NULL;          /* temporary vector for next step */
static idx_t computed_max = 0;          /* highest n for which a(n) is known */
static int initialized = 0;

/* initialise adjacency matrix and DP vectors */
static void init(void) {
    if (initialized) return;

    /* enumerate all states */
    size_t idx = 0;
    for (uint8_t a = 0; a <= MAX_VAL; ++a) {
        for (uint8_t b = 0; b <= MAX_VAL; ++b) {
            if (uabs_diff(a, b) <= 1) {
                sx[idx] = a;
                sy[idx] = b;
                ++idx;
            }
        }
    }

    /* build adjacency matrix */
    for (size_t i = 0; i < NUM_STATES; ++i) {
        uint8_t a = sx[i], b = sy[i];
        for (size_t j = 0; j < NUM_STATES; ++j) {
            uint8_t c = sx[j], d = sy[j];
            /* all cross‑differences must be ≤1; diagonal differences already checked by state definition */
            if (uabs_diff(a, c) <= 1 &&
                uabs_diff(b, d) <= 1 &&
                uabs_diff(a, d) <= 1 &&
                uabs_diff(b, c) <= 1) {
                adj[i][j] = 1;
            }
        }
    }

    /* allocate DP vectors */
    cur_vec = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
    next_vec = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));

    /* start with one row: one path of length 1 in each state */
    for (size_t i = 0; i < NUM_STATES; ++i)
        cur_vec[i] = 1;

    /* initialise a_values array */
    a_capacity = 8; /* arbitrary start size, will grow */
    a_values = (seq_t *)malloc((a_capacity + 1) * sizeof(seq_t));
    a_values[0] = 0;      /* unused, as sequence starts at n=1 */
    computed_max = 0;     /* no terms computed yet */

    /* mark initialized */
    initialized = 1;
}

/* ensure that a(n) is computed for all n up to the requested index */
static void ensure_computed(idx_t n) {
    init();

    if (n <= computed_max) return;

    for (idx_t step = computed_max + 1; step <= n; ++step) {
        /* compute next_vec from cur_vec */
        for (size_t j = 0; j < NUM_STATES; ++j) next_vec[j] = 0;
        for (size_t i = 0; i < NUM_STATES; ++i) {
            if (!cur_vec[i]) continue;
            for (size_t j = 0; j < NUM_STATES; ++j) {
                if (adj[i][j]) {
                    next_vec[j] += cur_vec[i];
                }
            }
        }

        /* sum all paths of this length */
        seq_t total = 0;
        for (size_t j = 0; j < NUM_STATES; ++j) total += next_vec[j];

        /* store result */
        if ((size_t)step >= a_capacity) {
            size_t newcap = a_capacity ? a_capacity * 2 : 8;
            while (newcap <= (size_t)step) newcap *= 2;
            a_values = (seq_t *)realloc(a_values, (newcap + 1) * sizeof(seq_t));
            a_capacity = newcap;
        }
        a_values[step] = total;

        /* prepare for next iteration */
        for (size_t i = 0; i < NUM_STATES; ++i) cur_vec[i] = next_vec[i];
    }

    computed_max = n;
}

/* public function ---------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;   /* not used in OEIS, but safe */
    ensure_computed(n);
    return a_values[n];
}
