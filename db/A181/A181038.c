/*
 * A181038  Number of nX4 binary matrices with no initial bit string in any
 *          row or column divisible by 6.
 * https://oeis.org/A181038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of n×4 binary matrices with no initial bit string
   in any row or column divisible by 6.
   Computed by dynamic programming over column remainders. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Constants for the automaton */
#define BASE 6          /* modulo value */
#define COLS 4          /* number of columns */
#define STATE_COUNT (BASE * BASE * BASE * BASE)  /* 6^4 = 1296 */

/* Allowed 4‑bit row patterns (in binary) with no prefix divisible by 6. */
static const uint8_t allowed_rows[6] = {8, 9, 10, 11, 14, 15};  /* 1000,1001,1010,1011,1110,1111 */

/* Decode tables: remainder of each column for a given state. */
static uint8_t r0[STATE_COUNT];
static uint8_t r1[STATE_COUNT];
static uint8_t r2[STATE_COUNT];
static uint8_t r3[STATE_COUNT];

/* Transition: for a state s and an allowed row index i,
   the new state (or -1 if a remainder would become 0). */
static int trans[STATE_COUNT][6];

/* DP arrays */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Cache of a(n) values */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Flag to initialise on first call */
static bool initialized = false;

/* Encode a state from 4 column remainders (0..5). */
static inline int encode_state(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return a + b * BASE + c * BASE * BASE + d * BASE * BASE * BASE;
}

/* Initialise tables and DP arrays. */
static void init(void)
{
    /* Decode each state into its column remainders. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int tmp = s;
        r0[s] = tmp % BASE; tmp /= BASE;
        r1[s] = tmp % BASE; tmp /= BASE;
        r2[s] = tmp % BASE; tmp /= BASE;
        r3[s] = tmp % BASE;
    }

    /* Precompute transitions. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        uint8_t a0 = r0[s];
        uint8_t a1 = r1[s];
        uint8_t a2 = r2[s];
        uint8_t a3 = r3[s];
        for (int i = 0; i < 6; ++i) {
            uint8_t pat = allowed_rows[i];
            uint8_t b0 = (pat >> 3) & 1;
            uint8_t b1 = (pat >> 2) & 1;
            uint8_t b2 = (pat >> 1) & 1;
            uint8_t b3 = pat & 1;

            uint8_t n0 = (a0 * 2 + b0) % BASE;
            uint8_t n1 = (a1 * 2 + b1) % BASE;
            uint8_t n2 = (a2 * 2 + b2) % BASE;
            uint8_t n3 = (a3 * 2 + b3) % BASE;

            /* Any zero remainder invalidates the transition. */
            if (n0 == 0 || n1 == 0 || n2 == 0 || n3 == 0) {
                trans[s][i] = -1;
            } else {
                trans[s][i] = encode_state(n0, n1, n2, n3);
            }
        }
    }

    /* Allocate DP arrays. */
    dp_cur  = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    dp_next = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;            /* start state: all remainders 0 */

    initialized = true;
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!initialized) init();

    /* Extend cache to include a(n). */
    while (cache_len < (size_t)n) {
        /* zero next DP array */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* propagate DP */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp_cur[s];
            if (cur == 0) continue;
            for (int i = 0; i < 6; ++i) {
                int t = trans[s][i];
                if (t < 0) continue;
                dp_next[t] += cur;
            }
        }

        /* sum counts for all states (no state has remainder 0) */
        seq_t ans = 0;
        for (int i = 0; i < STATE_COUNT; ++i) ans += dp_next[i];

        /* store the answer */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = ans;

        /* swap current and next DP arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return cache[n - 1];
}
