/*
 * A200465  Number of 0..4 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its four previous neighbors modulo 5.
 * https://oeis.org/A200465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..4 arrays of length n in which each element
 *            is at least as large as the sum of its four previous neighbors
 *            modulo 5. (Defined for n ≥ 1.)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"

/* Number of possible states: 5^4 = 625. */
#define STATE_COUNT 625

/* Transition data for all states (max 5 next states per state). */
static int next_count[STATE_COUNT];
static int next_state[STATE_COUNT][5];
static bool trans_init = false;

/* Initialise transition table */
static void init_transitions(void) {
    if (trans_init) return;
    for (int idx = 0; idx < STATE_COUNT; ++idx) {
        int a = idx / 125;
        int rem = idx % 125;
        int b = rem / 25;
        rem = rem % 25;
        int c = rem / 5;
        int d = rem % 5;
        int sum = (a + b + c + d) % 5;
        int cnt = 0;
        for (int e = sum; e <= 4; ++e) {
            int nxt = ((b * 5 + c) * 5 + d) * 5 + e;
            next_state[idx][cnt++] = nxt;
        }
        next_count[idx] = cnt;
    }
    trans_init = true;
}

/* Dynamic programming state containers */
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;

/* Cache of computed a(n) values */
static seq_t *a_cache = NULL;
static idx_t cache_size = 0; /* largest n for which a_cache contains a value */

/* Main function required by the template */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1 but harmless */

    if (!trans_init) init_transitions();

    /* Allocate DP arrays on first use */
    if (!dp_prev) {
        dp_prev = alloc_seq_array(STATE_COUNT);
        dp_next = alloc_seq_array(STATE_COUNT);
        for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = 0;
        dp_prev[0] = 1;   /* state (0,0,0,0) – before any entries */
    }

    /* Extend cache up to n if necessary */
    if (cache_size < n) {
        /* Grow cache array */
        size_t new_size = (size_t)(n + 1); /* keep index n reachable */
        seq_t *new_cache = alloc_seq_array(new_size);
        /* Copy old values if any */
        for (size_t i = 0; i <= cache_size; ++i)
            new_cache[i] = a_cache ? a_cache[i] : 0;
        a_cache = new_cache;

        /* Compute values for steps (cache_size+1 .. n) */
        for (idx_t step = cache_size + 1; step <= n; ++step) {
            /* Zero dp_next */
            for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;
            /* Transition */
            for (int s = 0; s < STATE_COUNT; ++s) {
                seq_t val = dp_prev[s];
                if (!val) continue;
                int cnt = next_count[s];
                for (int k = 0; k < cnt; ++k) {
                    int nxt = next_state[s][k];
                    dp_next[nxt] += val;
                }
            }
            /* Swap dp_prev and dp_next */
            seq_t *tmp = dp_prev;
            dp_prev = dp_next;
            dp_next = tmp;
            /* Sum total ways for this step */
            seq_t total = 0;
            for (int s = 0; s < STATE_COUNT; ++s) total += dp_prev[s];
            a_cache[step] = total;
        }
        cache_size = n;
    }
    return a_cache[n];
}
