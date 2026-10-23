/*
 * A164453  Number of binary strings of length n with no substrings equal to
 *          0001 0011 or 0100.
 * https://oeis.org/A164453
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that avoid the *
 *            forbidden substrings 0001, 0011, and 0100. * The implementation
 *            uses a deterministic finite automaton (DFA) * to count the
 *            admissible strings by dynamic programming
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define NUM_STATES 7

/* state indices */
#define STATE_EMPTY 0
#define STATE_0     1
#define STATE_00    2
#define STATE_000   3
#define STATE_001   4
#define STATE_01    5
#define STATE_010   6

/* transition table: trans[s][b] = next state on bit b (0 or 1),
 * -1 indicates a forbidden transition (dead state). */
static const signed char trans[NUM_STATES][2] = {
    { STATE_0,    STATE_EMPTY   },   /* ""   */
    { STATE_00,   STATE_01      },   /* "0"  */
    { STATE_000,  STATE_001     },   /* "00" */
    { STATE_000,  -1            },   /* "000"*/
    { STATE_010,  -1            },   /* "001"*/
    { STATE_010,  STATE_EMPTY   },   /* "01" */
    { -1,         STATE_01      }    /* "010"*/
};

/* cache of computed a(n) values: cache[0] .. cache[current_len] */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* current automaton state counts for the longest length computed
 * so far (current_len). */
static seq_t state_vec[NUM_STATES] = {0};
static idx_t current_len = 0;

/* ensure that cache[0 … n] are computed.  The routine grows the
 * cache only if needed and extends the DP one step at a time.
 * It works correctly regardless of the order in which a(n) is called. */
static void ensure_cache(idx_t n)
{
    /* first call: initialize cache and basis state vector */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;                     /* a(0) = 1 (empty string) */
        for (int s = 0; s < NUM_STATES; ++s) state_vec[s] = (seq_t)0;
        state_vec[STATE_EMPTY] = (seq_t)1;        /* state for length 0 */
        current_len = 0;
    }

    if (n <= current_len) return;

    /* grow cache array if necessary */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* extend DP from current_len+1 up to n */
    for (idx_t len = current_len + 1; len <= n; ++len) {
        seq_t next[NUM_STATES] = {0};

        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t ways = state_vec[s];
            if (!ways) continue;
            for (int b = 0; b < 2; ++b) {
                int ns = trans[s][b];
                if (ns >= 0) next[ns] += ways;
            }
        }
        seq_t sum = 0;
        for (int s = 0; s < NUM_STATES; ++s) sum += next[s];
        cache[(size_t)len] = sum;

        /* copy next to state_vec for next iteration */
        for (int s = 0; s < NUM_STATES; ++s) state_vec[s] = next[s];
    }
    current_len = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
