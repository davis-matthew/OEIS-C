/*
 * A197669  Number of nX5 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,0,1,0,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A197669 – Number of n×5 arrays over {0,1,2,3,4} where each entry
 * x is equal to the number of its horizontal and vertical neighbours
 * equal to 4,0,1,0,1 for x=0,1,2,3,4 respectively.
 *
 * This implementation builds a deterministic finite automaton with
 * 5⁵ × 2⁵ = 100 000 states and propagates counts column by column.
 * The result is the sum of states whose residual mask is zero.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ----- Parameters ------------------------------------------------- */
#define ROWS         5                /* number of rows in the array */
#define PATTERN_CNT  3125             /* 5⁵ possible column patterns */
#define MASK_CNT     32               /* 2⁵ possible residual masks   */
#define STATE_CNT    (PATTERN_CNT * MASK_CNT)
#define WORDS        ((PATTERN_CNT + 63) >> 6)   /* number of 64‑bit words */

/* ----- Pattern tables ---------------------------------------------- */
static uint8_t pattern_val[PATTERN_CNT][ROWS];      /* digits 0..4            */
static uint8_t pattern_target[PATTERN_CNT][ROWS];   /* target value for a cell */
static uint8_t pattern_req[PATTERN_CNT][ROWS];      /* required neighbour count */
static uint8_t pattern_updown[PATTERN_CNT][ROWS];   /* vertical neighbour count */

static uint64_t bits_row_val[ROWS][5][WORDS];        /* bitsets per row/value */
static uint64_t all_mask[WORDS];                    /* full‑ones mask */

/* ----- Transition list ------------------------------------------- */
typedef struct {
    uint32_t *data;   /* packed next_state = (pattern<<5) | mask */
    size_t   len;
    size_t   cap;
} TransitionList;

static TransitionList transitions[STATE_CNT];

/* ----- Helper: push a transition --------------------------------- */
static inline void push_transition(int from_idx, int pattern, int mask)
{
    TransitionList *t = &transitions[from_idx];
    if (t->len == t->cap) {
        size_t newcap = t->cap ? t->cap * 2 : 16;
        uint32_t *newdata = (uint32_t *)realloc(t->data, newcap * sizeof(uint32_t));
        t->data = newdata;
        t->cap  = newcap;
    }
    t->data[t->len++] = ((uint32_t)pattern << 5) | (uint32_t)mask;
}

/* ----- One‑time table initialization ------------------------------ */
static bool tables_ready = false;

static void init_tables(void)
{
    const uint8_t target_for_val[5] = {4,0,1,0,1};
    const uint8_t req_for_val[5]    = {0,1,2,3,4};

    /* Zero out bitsets */
    for (int r = 0; r < ROWS; ++r)
        for (int v = 0; v < 5; ++v)
            memset(bits_row_val[r][v], 0, WORDS * sizeof(uint64_t));

    /* Build pattern data and per‑row/value bitsets */
    for (int p = 0; p < PATTERN_CNT; ++p) {
        int tmp = p;
        for (int r = 0; r < ROWS; ++r) {
            uint8_t v = tmp % 5;
            tmp /= 5;
            pattern_val[p][r]      = v;
            pattern_target[p][r]   = target_for_val[v];
            pattern_req[p][r]      = req_for_val[v];
        }
        /* Count vertical neighbours equal to target */
        for (int r = 0; r < ROWS; ++r) {
            uint8_t t = pattern_target[p][r];
            unsigned int cnt = 0;
            if (r > 0   && pattern_val[p][r-1] == t) ++cnt;
            if (r < ROWS-1 && pattern_val[p][r+1] == t) ++cnt;
            pattern_updown[p][r] = (uint8_t)cnt;
        }
        /* Set bits in per‑row/value bitsets */
        int word = p >> 6;                 /* p / 64 */
        uint64_t bit = 1ULL << (p & 63);   /* p % 64 */
        for (int r = 0; r < ROWS; ++r) {
            uint8_t v = pattern_val[p][r];
            bits_row_val[r][v][word] |= bit;
        }
    }

    /* Build mask of all 1's for used bits */
    for (int w = 0; w < WORDS; ++w) {
        if (w == WORDS - 1) {
            int rem = PATTERN_CNT - w * 64;
            all_mask[w] = (rem == 64) ? ~0ULL : ((1ULL << rem) - 1);
        } else {
            all_mask[w] = ~0ULL;
        }
    }

    /* Pre‑compute transition lists for every state */
    uint64_t allowed[WORDS];
    for (int q = 0; q < PATTERN_CNT; ++q) {
        for (int mask = 0; mask < MASK_CNT; ++mask) {
            /* Start with all patterns */
            if (mask & 1) { /* first row requires equality */
                memcpy(allowed, bits_row_val[0][pattern_target[q][0]],
                       WORDS * sizeof(uint64_t));
            } else { /* first row must differ from target */
                for (int w = 0; w < WORDS; ++w)
                    allowed[w] = (~bits_row_val[0][pattern_target[q][0]][w]) & all_mask[w];
            }
            for (int r = 1; r < ROWS; ++r) {
                bool equal = (mask >> r) & 1;
                uint64_t *bits = bits_row_val[r][pattern_target[q][r]];
                if (equal) {
                    for (int w = 0; w < WORDS; ++w)
                        allowed[w] &= bits[w];
                } else {
                    for (int w = 0; w < WORDS; ++w)
                        allowed[w] &= (~bits[w]) & all_mask[w];
                }
            }

            int state_index = q * MASK_CNT + mask;
            for (int w = 0; w < WORDS; ++w) {
                uint64_t word_val = allowed[w];
                while (word_val) {
                    uint64_t lowbit = word_val & -word_val;
                    int bitpos = __builtin_ctzll(word_val);
                    int p = (w << 6) + bitpos;   /* candidate pattern id */
                    if (p < PATTERN_CNT) {
                        /* Compute residual mask for the next column. */
                        int new_mask = 0;
                        bool ok = true;
                        for (int r = 0; r < ROWS; ++r) {
                            bool left_match = (pattern_val[q][r] == pattern_target[p][r]);
                            int need = (int)pattern_req[p][r]
                                     - (int)pattern_updown[p][r]
                                     - (left_match ? 1 : 0);
                            if (need != 0 && need != 1) {
                                ok = false;
                                break;
                            }
                            if (need == 1)
                                new_mask |= 1 << r;
                        }
                        if (ok)
                            push_transition(state_index, p, new_mask);
                    }
                    word_val ^= lowbit;
                }
            }
        }
    }

    tables_ready = true;
}

/* ----- State arrays for DP -------------------------------------- */
static seq_t *cur = NULL;
static seq_t *nxt = NULL;
static int  *active = NULL;
static int  *next_active = NULL;
static size_t max_active = 0;

/* ----- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (!tables_ready)
        init_tables();

    if (cur == NULL) {
        cur  = (seq_t *)calloc(STATE_CNT, sizeof(seq_t));
        nxt  = (seq_t *)calloc(STATE_CNT, sizeof(seq_t));
        active      = (int *)malloc(STATE_CNT * sizeof(int));
        next_active = (int *)malloc(STATE_CNT * sizeof(int));
        max_active  = STATE_CNT;
    }

    /* Initialise for the first column (no left neighbours) */
    size_t active_cnt = 0;
    memset(cur, 0, STATE_CNT * sizeof(seq_t));
    for (int p = 0; p < PATTERN_CNT; ++p) {
        int mask = 0, ok = 1;
        for (int r = 0; r < ROWS; ++r) {
            int need = (int)pattern_req[p][r] - (int)pattern_updown[p][r];
            if (need < 0 || need > 1) { ok = 0; break; }
            if (need == 1)
                mask |= 1 << r;
        }
        if (ok) {
            int idx = p * MASK_CNT + mask;
            cur[idx] = 1;
            active[active_cnt++] = idx;
        }
    }

    /* Process remaining columns */
    for (idx_t col = 1; col < n; ++col) {
        size_t next_cnt = 0;
        memset(nxt, 0, STATE_CNT * sizeof(seq_t));

        for (size_t ai = 0; ai < active_cnt; ++ai) {
            int idx = active[ai];
            seq_t ways = cur[idx];
            if (ways == 0) continue;

            TransitionList *t = &transitions[idx];
            for (size_t ti = 0; ti < t->len; ++ti) {
                uint32_t enc = t->data[ti];
                int nxt_idx = enc;
                if (nxt[nxt_idx] == 0)
                    next_active[next_cnt++] = nxt_idx;
                nxt[nxt_idx] += ways;
            }
            cur[idx] = 0;  /* clean up for next use */
        }

        /* Swap buffers and active lists */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
        int *tmpi = active; active = next_active; next_active = tmpi;
        active_cnt = next_cnt;
    }

    /* Sum counts whose residual mask is zero (no right neighbours required) */
    seq_t result = 0;
    for (size_t ai = 0; ai < active_cnt; ++ai) {
        int idx = active[ai];
        if ((idx & 31) == 0)   /* mask == 0 */
            result += cur[idx];
    }
    return result;
}
