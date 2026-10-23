/*
 * A187922  Positions k of addition steps in Recamán's sequence where
 *          A005132(k-1)-k = A005132(m) for some 0 < m < k.
 * https://oeis.org/A187922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4258, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A187922 – positions k of addition steps in Recamán's sequence where
 * A005132(k-1)-k equals a previous A005132 value (with 0 < m < k).
 * The function generates Recamán's sequence on demand, storing seen
 * values and the qualifying indices. */

#include <stddef.h>     /* size_t, NULL */
#include <stdint.h>     /* uint64_t */
#include <stdlib.h>     /* malloc, realloc, free, exit */
#include <string.h>     /* memset */
#include <stdbool.h>    /* bool, true, false */

#ifdef __cplusplus
#include <set>
#endif

/* ---------- Presence test: set or bitset ---------- */
#ifdef __cplusplus

/* C++ build: use std::set */
static std::set<seq_t> seen;

static inline bool seen_contains(const seq_t &v) { return seen.find(v) != seen.end(); }
static inline void seen_add(const seq_t &v)       { seen.insert(v); }

#else
/* C build: use dynamic bitset */
static uint64_t *bitset = NULL;   /* array of 64‑bit words */
static size_t bitset_words = 0;   /* number of words allocated */

static inline void ensure_bitset_size(seq_t v)
{
    /* Bits needed to hold 'v' */
    size_t needed = (size_t)(v >> 6) + 1;
    if (needed <= bitset_words) return;

    size_t new_words = bitset_words ? bitset_words : 1;
    while (new_words < needed) new_words <<= 1;

    uint64_t *new_arr = (uint64_t *)realloc(bitset, new_words * sizeof(uint64_t));
    if (!new_arr) exit(EXIT_FAILURE);          /* allocation failed */

    /* zero new words */
    memset(new_arr + bitset_words, 0,
           (new_words - bitset_words) * sizeof(uint64_t));

    bitset = new_arr;
    bitset_words = new_words;
}

static inline bool seen_contains(seq_t v)
{
    size_t word = (size_t)(v >> 6);
    if (word >= bitset_words) return false;
    return ((bitset[word] >> (v & 63)) & 1U) != 0;
}

static inline void seen_add(seq_t v)
{
    ensure_bitset_size(v);
    size_t word = (size_t)(v >> 6);
    bitset[word] |= 1ULL << (v & 63);
}
#endif

/* ---------- Dynamic array for recorded positions ---------- */
static idx_t *pos_arr = NULL;   /* indices at which addition occurs with positive subvalue */
static size_t pos_cap = 0;
static idx_t pos_len = 0;

static inline void add_position(idx_t v)
{
    if (pos_len == pos_cap) {
        size_t new_cap = pos_cap ? pos_cap * 2 : 8;
        idx_t *new_arr =
            (idx_t *)realloc(pos_arr, new_cap * sizeof(idx_t));
        if (!new_arr) exit(EXIT_FAILURE);
        pos_arr = new_arr;
        pos_cap = new_cap;
    }
    pos_arr[pos_len++] = v;
}

/* ---------- Recamán state ---------- */
static seq_t cur_val = 0;     /* current a(n) value  */
static idx_t cur_index = 0;   /* current n (0‑based) */
static bool initialized = false;

static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;
    seen_add((seq_t)0);   /* a(0)=0 is already present */
}

/* ---------- Generate next Recamán term and record qualifying indices ---------- */
static void generate_next(void)
{
    cur_index++;                    /* move to next step (n = cur_index) */
    seq_t step = (seq_t)cur_index;  /* n in Recamán's definition */

    bool sub_possible = cur_val >= step;
    seq_t sub_val = 0;

    /* Attempt subtraction */
    if (sub_possible) {
        sub_val = cur_val - step;
        if (!seen_contains(sub_val)) {
            /* subtraction allowed */
            cur_val = sub_val;
            seen_add(cur_val);
            return;  /* not an addition step */
        }
    }

    /* Addition step */
    cur_val = cur_val + step;
    seen_add(cur_val);

    /* Record k if subtracted value > 0 and already present */
    if (sub_possible && sub_val != 0) {
        /* Positions are 1‑based in OEIS A187922 */
        add_position(cur_index);
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    init_sequence();

    while (pos_len < n) {
        generate_next();
    }
    return pos_arr[n - 1];
}
