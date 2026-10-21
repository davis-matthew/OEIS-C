/*
 * A239410  Number of 6Xn 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 6×n arrays over {0,1,2,3} with the constraint that
 *            no element equals one plus the sum of elements to its left mod
 *            4, nor two plus the sum of the elements above it mod 4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper constants */
#define ROWS 6
#define STATES (1 << (2 * ROWS))          /* 4^6 = 4096 */
#define MAX_EDGES (STATES * 730)         /* upper bound on edges per row */

/* Adjacency data */
static int start[STATES + 1];
static int edges[MAX_EDGES];
static int edge_count = 0;

/* Precomputed digits of each state */
static uint8_t state_digits[STATES][ROWS];

/* Valid vertical vectors (satisfy vertical constraints only) */
static uint8_t verticals[729][ROWS];
static int vertical_cnt = 0;

/* DP buffers */
static seq_t *curr = NULL;
static seq_t *next = NULL;

/* Value cache */
static seq_t *values = NULL;
static idx_t values_cap = 0;
static idx_t values_count = 0;

/* Current maximum n computed */
static idx_t max_computed = 0;

/* Forward declarations */
static void build_vertical_vectors(void);
static void build_adjacency(void);
static void init_buffers(void);

/* Recursive generator for vertical vectors */
static void gen_vertical(int r, uint8_t sum, uint8_t vec[ROWS])
{
    if (r == ROWS) {
        memcpy(verticals[vertical_cnt], vec, ROWS);
        ++vertical_cnt;
        return;
    }
    for (int v = 0; v < 4; ++v) {
        if (r == 0) {
            if (v == 2) continue;          /* v[0] != 2 */
        } else {
            if (v == ((2 + sum) & 3)) continue; /* v[r] != 2 + sum_above */
        }
        vec[r] = (uint8_t)v;
        gen_vertical(r + 1, (sum + v) & 3, vec);
    }
}

/* Build all precomputed structures the first time a(n) is called */
static void build_vertical_vectors(void)
{
    uint8_t tmp[ROWS];
    gen_vertical(0, 0, tmp);
}

static void build_adjacency(void)
{
    /* Build state_digits */
    for (int s = 0; s < STATES; ++s) {
        int tmp = s;
        for (int r = 0; r < ROWS; ++r) {
            state_digits[s][r] = tmp & 3;
            tmp >>= 2;
        }
    }

    edge_count = 0;
    for (int s = 0; s < STATES; ++s) {
        start[s] = edge_count;
        const uint8_t *p = state_digits[s];
        for (int vi = 0; vi < vertical_cnt; ++vi) {
            const uint8_t *v = verticals[vi];
            int i;
            for (i = 0; i < ROWS; ++i) {
                if (v[i] == ((p[i] + 1) & 3)) break;   /* left constraint fail */
            }
            if (i != ROWS) continue; /* invalid vector */

            /* Compute resulting state */
            int t = 0;
            for (int r = 0; r < ROWS; ++r) {
                uint8_t val = (p[r] + v[r]) & 3;
                t |= (val << (2 * r));
            }
            edges[edge_count++] = t;
        }
    }
    start[STATES] = edge_count;
}

static void init_buffers(void)
{
    /* Allocate DP buffers */
    curr = (seq_t*)calloc(STATES, sizeof(seq_t));
    next = (seq_t*)calloc(STATES, sizeof(seq_t));
    curr[0] = 1;  /* start state */

    /* Allocate values cache */
    values_cap = 64;
    values = (seq_t*)malloc(values_cap * sizeof(seq_t));
    values[0] = 0; /* unused, as offset is 1 */
    values_count = 1; /* only values[0] defined */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Lazy initialization */
    if (curr == NULL) {
        build_vertical_vectors();
        build_adjacency();
        init_buffers();
    }

    /* Extend cache if needed */
    if (n >= values_cap) {
        idx_t new_cap = values_cap == 0 ? 64 : values_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_vals = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
        if (!new_vals) abort(); /* allocation failure */
        values = new_vals;
        values_cap = new_cap;
    }

    /* Compute up to requested n */
    for (idx_t step = max_computed + 1; step <= n; ++step) {
        /* Zero next buffer */
        memset(next, 0, STATES * sizeof(seq_t));

        /* Transition */
        for (int s = 0; s < STATES; ++s) {
            seq_t val = curr[s];
            if (val == 0) continue;
            for (int e = start[s]; e < start[s + 1]; ++e) {
                int t = edges[e];
                next[t] += val;
            }
        }

        /* Sum all entries to get a(step) */
        seq_t sum = 0;
        for (int i = 0; i < STATES; ++i) sum += next[i];
        values[step] = sum;

        /* Swap buffers */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }
    max_computed = n;

    return values[n];
}
