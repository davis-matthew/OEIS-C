/*
 * A219926  Number of tilings of a 7 X n rectangle using integer-sided square
 *          tiles.
 * https://oeis.org/A219926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = number of tilings of a 7×n rectangle with integer‑sided
 * square tiles.  Computed by a transfer‑matrix dynamic programme on the
 * 7‑row column states.  The state graph is built lazily on the first call.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define HEIGHT   7
#define MAX_KEY  823543   /* 7^7 */

/* ------------------------------------------------------------ */

static int key_to_index[MAX_KEY];
static int pow7[HEIGHT];

typedef struct {
    uint8_t cnt[HEIGHT];   /* remaining width of horizontal tiles in each row */
    int *trans;            /* array of transition destinations */
    int   trans_cap;
    int   trans_cnt;
    int   processed;        /* 0 = not processed, 1 = processed */
} State;

static State *states   = NULL;
static size_t state_cap = 0;
static size_t state_cnt = 0;
static int zero_state_index = -1;
static bool built = false;

/* ------------------------------------------------------------ */

static void init_pow7(void) {
    pow7[0] = 1;
    for (int i = 1; i < HEIGHT; ++i)
        pow7[i] = pow7[i - 1] * 7;
}

/* ------------------------------------------------------------ */

static void add_state(const uint8_t cnt[HEIGHT], int *idx_out) {
    int key = 0;
    for (int i = 0; i < HEIGHT; ++i)
        key += cnt[i] * pow7[i];

    if (key_to_index[key] != -1) {
        *idx_out = key_to_index[key];
        return;
    }

    if (state_cnt == state_cap) {
        state_cap = state_cap ? state_cap * 2 : 64;
        states = (State *)realloc(states, state_cap * sizeof(State));
    }
    State *s = &states[state_cnt];
    memcpy(s->cnt, cnt, HEIGHT);
    s->trans = NULL;
    s->trans_cap = 0;
    s->trans_cnt = 0;
    s->processed = 0;
    int new_idx = state_cnt++;
    key_to_index[key] = new_idx;
    *idx_out = new_idx;
}

/* ------------------------------------------------------------ */

static void add_transition(int from, int to) {
    State *s = &states[from];
    if (s->trans_cnt == s->trans_cap) {
        s->trans_cap = s->trans_cap ? s->trans_cap * 2 : 4;
        s->trans = (int *)realloc(s->trans, s->trans_cap * sizeof(int));
    }
    s->trans[s->trans_cnt++] = to;
}

/* ------------------------------------------------------------ */

static void generate_rec(int from_idx, const uint8_t cur[HEIGHT],
                         const uint8_t next_partial[HEIGHT], int pos) {
    if (pos == HEIGHT) {
        int to_idx;
        add_state(next_partial, &to_idx);
        add_transition(from_idx, to_idx);
        return;
    }

    if (cur[pos] > 0) {
        uint8_t next[HEIGHT];
        memcpy(next, next_partial, HEIGHT);
        next[pos] = cur[pos] - 1;
        generate_rec(from_idx, cur, next, pos + 1);
    } else {
        /* 1×1 tile */
        uint8_t next[HEIGHT];
        memcpy(next, next_partial, HEIGHT);
        generate_rec(from_idx, cur, next, pos + 1);

        /* larger squares */
        for (int k = 2; k <= HEIGHT - pos; ++k) {
            bool ok = true;
            for (int r = pos; r < pos + k; ++r)
                if (cur[r] != 0) { ok = false; break; }
            if (!ok) continue;

            uint8_t next2[HEIGHT];
            memcpy(next2, next_partial, HEIGHT);
            for (int r = pos; r < pos + k; ++r)
                next2[r] = k - 1;
            generate_rec(from_idx, cur, next2, pos + k);
        }
    }
}

/* ------------------------------------------------------------ */

static void build_graph(void) {
    for (int i = 0; i < MAX_KEY; ++i) key_to_index[i] = -1;
    init_pow7();

    uint8_t zero[HEIGHT] = {0};
    add_state(zero, &zero_state_index);   /* start state */

    while (true) {
        size_t changes = 0;
        for (size_t i = 0; i < state_cnt; ++i) {
            if (!states[i].processed) {
                states[i].processed = 1;
                uint8_t cur[HEIGHT];
                memcpy(cur, states[i].cnt, HEIGHT);
                uint8_t next_partial[HEIGHT] = {0};
                generate_rec((int)i, cur, next_partial, 0);
                ++changes;
            }
        }
        if (changes == 0) break;   /* all states processed */
    }
    built = true;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (!built) build_graph();

    seq_t *dp  = (seq_t *)calloc(state_cnt, sizeof(seq_t));
    seq_t *ndp = (seq_t *)calloc(state_cnt, sizeof(seq_t));
    dp[zero_state_index] = 1;

    for (idx_t col = 0; col < n; ++col) {
        for (size_t i = 0; i < state_cnt; ++i) ndp[i] = 0;

        for (size_t s = 0; s < state_cnt; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            State *st = &states[s];
            for (int t = 0; t < st->trans_cnt; ++t) {
                int to = st->trans[t];
                ndp[to] = ndp[to] + val;
            }
        }

        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    seq_t answer = dp[zero_state_index];
    free(dp);
    free(ndp);
    return answer;
}
