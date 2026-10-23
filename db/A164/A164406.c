/*
 * A164406  Number of binary strings of length n with no substrings equal to
 *          0011 or 0101.
 * https://oeis.org/A164406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that avoid
 *            substrings 0011 and 0101. The function implements the
 *            linear‑recurrence derived from the generating function
 *            (x^2+1)/(x^3+x-1)^2 via a finite automaton that counts strings
 *            with no forbidden 4‑bit pattern
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 49, u64 n <= 105, u128 n <= 219,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STATE_CNT 15   /* 1 + 2 + 4 + 8 states */

/* ------------------------------------------------------------ */
/* global static data for the automaton and memoisation       */
/* ------------------------------------------------------------ */
static bool state_initialized = false;
static int  state_id_of[4][8];         /* mapping (len,bits) -> id */
static uint8_t state_len[STATE_CNT];   /* length of state                               */
static uint8_t state_bits[STATE_CNT];  /* bits of state (0..7)                          */

static bool cache_initialized = false;
static seq_t *cache = NULL;            /* memoised a(n) values          */
static size_t cache_cap = 0;           /* capacity of cache            */
static idx_t  cache_last = (idx_t)-1;  /* largest n for which a(n) is memoised */

static seq_t dp_cur[STATE_CNT];        /* counts for current length   */
static seq_t dp_next[STATE_CNT];       /* counts for next length      */

/* ------------------------------------------------------------ */
/* initialise the automaton state tables                          */
/* ------------------------------------------------------------ */
static void init_automaton(void)
{
    if (state_initialized) return;
    int id = 0;

    /* length 0 */
    state_id_of[0][0] = id;
    state_len[id] = 0;   state_bits[id] = 0;
    id++;

    /* length 1 */
    for (int bits = 0; bits < 2; ++bits) {
        state_id_of[1][bits] = id;
        state_len[id] = 1;   state_bits[id] = bits;
        id++;
    }

    /* length 2 */
    for (int bits = 0; bits < 4; ++bits) {
        state_id_of[2][bits] = id;
        state_len[id] = 2;   state_bits[id] = bits;
        id++;
    }

    /* length 3 */
    for (int bits = 0; bits < 8; ++bits) {
        state_id_of[3][bits] = id;
        state_len[id] = 3;   state_bits[id] = bits;
        id++;
    }
    state_initialized = true;
}

/* ------------------------------------------------------------ */
/* initialise the global memoisation buffer and DP structures   */
/* ------------------------------------------------------------ */
static void init_cache(void)
{
    if (cache_initialized) return;

    cache_cap = 1;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)1;
    cache_last = 0;

    memset(dp_cur, 0, sizeof(dp_cur));
    dp_cur[0] = (seq_t)1;
    memset(dp_next, 0, sizeof(dp_next));

    cache_initialized = true;
}

/* ------------------------------------------------------------ */
/* extension of the allocation of memoised values              */
/* ------------------------------------------------------------ */
static void ensure_cache_cap(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (cache_cap >= target) return;
    size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
    while (new_cap < target) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* the required function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_automaton();
    init_cache();

    if (n <= cache_last)
        return cache[(size_t)n];

    ensure_cache_cap(n);

    /* compute all values from cache_last+1 up to n                    */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int s = 0; s < STATE_CNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;

            uint8_t len   = state_len[s];
            uint8_t bits  = state_bits[s];

            /* append bit 0 */
            if (len >= 3) {
                int pattern = (bits << 1);
                if (pattern != 3 && pattern != 5) {
                    int new_id = state_id_of[3][pattern & 7];
                    dp_next[new_id] += val;
                }
            } else {
                int new_len  = len + 1;
                int new_bits = (bits << 1);
                int new_id   = state_id_of[new_len][new_bits];
                dp_next[new_id] += val;
            }

            /* append bit 1 */
            if (len >= 3) {
                int pattern = (bits << 1) | 1;
                if (pattern != 3 && pattern != 5) {
                    int new_id = state_id_of[3][pattern & 7];
                    dp_next[new_id] += val;
                }
            } else {
                int new_len  = len + 1;
                int new_bits = (bits << 1) | 1;
                int new_id   = state_id_of[new_len][new_bits];
                dp_next[new_id] += val;
            }
        }

        /* sum all state counts → a(i)                                 */
        seq_t sum = 0;
        for (int s = 0; s < STATE_CNT; ++s) sum += dp_next[s];
        cache[(size_t)i] = sum;

        /* prepare for next iteration                                */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    cache_last = n;
    return cache[(size_t)n];
}
