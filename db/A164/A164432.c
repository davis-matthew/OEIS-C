/*
 * A164432  Number of binary strings of length n with no substrings equal to
 *          0000 0101 or 0110.
 * https://oeis.org/A164432
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A164432 – number of binary strings of length n that avoid
 * the forbidden 4‑bit patterns 0000, 0101, and 0110.
 *
 * Computation: a finite‑state automaton remembers the last up to three
 * bits.  DP counts how many strings end in each state.  The result
 * a(n) is the total number of strings of length n.  Results are
 * memoised so successive calls are O(1) after the first run.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

enum { MAX_STATES = 15 };

/* mapping (len,val) -> state index */
static int state_index[4][8];

/* reverse mapping: state index -> (len,val) */
static struct { unsigned len; unsigned val; } state_tbl[MAX_STATES];

/* transition table: state → bit → next state (or -1 if forbidden) */
static int trans_table[MAX_STATES][2];

/* DP arrays */
static seq_t dp_cur[MAX_STATES];
static seq_t dp_next[MAX_STATES];

/* cache of previously computed a(n) values */
static seq_t *results = NULL;
static size_t results_size = 0;      /* allocated size of results array */
static idx_t cur_n = (idx_t)-1;      /* highest length fully computed */

/* forbidden 4‑bit patterns */
static const unsigned forbidden[3] = {0b0000, 0b0101, 0b0110};

/* initialise automaton once */
static void init_automaton(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* build state tables */
    int idx = 0;
    for (unsigned len = 0; len <= 3; ++len) {
        unsigned maxv = 1u << len;
        for (unsigned val = 0; val < maxv; ++val) {
            state_index[len][val] = idx;
            state_tbl[idx].len = len;
            state_tbl[idx].val = val;
            ++idx;
        }
    }

    /* build transition table */
    for (int s = 0; s < MAX_STATES; ++s) {
        unsigned len = state_tbl[s].len;
        unsigned val = state_tbl[s].val;
        for (int b = 0; b <= 1; ++b) {
            unsigned next_len = len + 1;
            unsigned next_val_bits = (val << 1) | (unsigned)b;

            bool forbid = false;
            if (next_len > 3) {
                unsigned last4 = next_val_bits & 0xF;
                for (int i = 0; i < 3; ++i)
                    if (last4 == forbidden[i]) { forbid = true; break; }

                next_val_bits &= 0x7;
                next_len = 3;
            } else {
                next_val_bits &= (1u << next_len) - 1;
            }

            if (forbid)
                trans_table[s][b] = -1;
            else
                trans_table[s][b] = state_index[next_len][next_val_bits];
        }
    }

    /* allocate cache for length 0 */
    results = (seq_t *)malloc(sizeof(seq_t));
    if (!results) abort();
    results[0] = (seq_t)1;          /* empty string */
    results_size = 1;

    /* initialise DP for length 0 */
    for (int s = 0; s < MAX_STATES; ++s) dp_cur[s] = (seq_t)0;
    dp_cur[0] = (seq_t)1;            /* state (len=0,val=0) */
    cur_n = 0;
}

/* enlarge results array if necessary */
static void ensure_results(size_t n)
{
    if (n < results_size) return;
    size_t new_size = results_size ? results_size * 2 : 64;
    while (new_size <= n) new_size *= 2;
    seq_t *tmp = (seq_t *)realloc(results, new_size * sizeof(seq_t));
    if (!tmp) abort();
    results = tmp;
    for (size_t i = results_size; i < new_size; ++i) results[i] = (seq_t)0;
    results_size = new_size;
}

/* advance DP one step */
static void dp_step(void)
{
    for (int s = 0; s < MAX_STATES; ++s) dp_next[s] = (seq_t)0;
    for (int s = 0; s < MAX_STATES; ++s) {
        seq_t val = dp_cur[s];
        if (!val) continue;
        for (int b = 0; b <= 1; ++b) {
            int ns = trans_table[s][b];
            if (ns >= 0) dp_next[ns] += val;
        }
    }
    for (int s = 0; s < MAX_STATES; ++s) dp_cur[s] = dp_next[s];
}

/* public sequence function */
seq_t a(idx_t n)
{
    init_automaton();
    if (n < cur_n) return results[n];

    for (idx_t len = cur_n + 1; len <= n; ++len) {
        dp_step();
        seq_t sum = (seq_t)0;
        for (int s = 0; s < MAX_STATES; ++s) sum += dp_cur[s];
        ensure_results((size_t)len);
        results[len] = sum;
        cur_n = len;
    }
    return results[n];
}
