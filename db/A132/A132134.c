/*
 * A132134  Base 3 "Punctual Bird" numbers: write the natural numbers, base 3,
 *          in a string 12101112202122100101102... Sequence gives numbers
 *          which do not occur in the string ahead of their natural place.
 * https://oeis.org/A132134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth natural number which, when its base‑3 * digits
 *            are appended to the infinite string formed by concatenating *
 *            base‑3 representations of all natural numbers starting from 1, *
 *            does not appear as a substring earlier in that string
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a suffix automaton to test early occurrences
 * in O(length) time per number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of seq_t and idx_t are given by the compiler. */

typedef struct {
    int next[3];
    int link;
    int len;
} SamState;

static SamState *sam_states = NULL;
static size_t sam_cap = 0;
static size_t sam_sz = 0;
static int sam_last = 0;

static void sam_init(void) {
    sam_cap = 1 << 18;            /* about 262k states */
    sam_states = (SamState *)malloc(sizeof(SamState) * sam_cap);
    /* root state */
    for (int j = 0; j < 3; ++j) sam_states[0].next[j] = -1;
    sam_states[0].link = -1;
    sam_states[0].len  = 0;
    sam_sz = 1;
    sam_last = 0;
}

static void sam_grow(void) {
    size_t new_cap = sam_cap * 2;
    SamState *new_arr = (SamState *)realloc(sam_states, sizeof(SamState) * new_cap);
    if (!new_arr) { exit(1); }
    sam_states = new_arr;
    sam_cap = new_cap;
}

static void sam_extend(int c) {
    if (sam_sz == sam_cap) sam_grow();
    int cur = (int)sam_sz++;
    sam_states[cur].len = sam_states[sam_last].len + 1;
    for (int j = 0; j < 3; ++j) sam_states[cur].next[j] = -1;
    int p = sam_last;
    while (p != -1 && sam_states[p].next[c] == -1) {
        sam_states[p].next[c] = cur;
        p = sam_states[p].link;
    }
    if (p == -1) {
        sam_states[cur].link = 0;
    } else {
        int q = sam_states[p].next[c];
        if (sam_states[p].len + 1 == sam_states[q].len) {
            sam_states[cur].link = q;
        } else {
            if (sam_sz == sam_cap) sam_grow();
            int clone = (int)sam_sz++;
            sam_states[clone] = sam_states[q];   /* copy fields */
            sam_states[clone].len = sam_states[p].len + 1;
            while (p != -1 && sam_states[p].next[c] == q) {
                sam_states[p].next[c] = clone;
                p = sam_states[p].link;
            }
            sam_states[q].link = sam_states[cur].link = clone;
        }
    }
    sam_last = cur;
}

/* Sequence storage */
static seq_t *punctual = NULL;
static size_t p_cap = 0;
static size_t p_sz  = 0;

static void p_grow(void) {
    size_t new_cap = p_cap * 2;
    if (new_cap == 0) new_cap = 64;
    seq_t *new_arr = (seq_t *)realloc(punctual, sizeof(seq_t) * new_cap);
    if (!new_arr) { exit(1); }
    punctual = new_arr;
    p_cap = new_cap;
}

static void p_append(seq_t v) {
    if (p_sz == p_cap) p_grow();
    punctual[p_sz++] = v;
}

/* State of the incremental construction */
static idx_t next_to_process = 1;   /* next natural number to test */

/* Public sequence function */
seq_t a(idx_t n) {
    static bool init_done = false;
    if (!init_done) {
        sam_init();
        p_cap = 0;
        p_sz = 0;
        next_to_process = 1;
        init_done = true;
    }

    while (p_sz < n) {
        idx_t cur = next_to_process;
        /* convert cur to base‑3 digits into array 'dig' */
        int dig[64];
        int len = 0;
        idx_t temp = cur;
        while (temp > 0) {
            dig[len++] = (int)(temp % 3);
            temp /= 3;
        }
        /* if cur == 0 (not used here) would need special handling */

        /* reverse to get correct order */
        for (int i = 0; i < len / 2; ++i) {
            int t = dig[i];
            dig[i] = dig[len - 1 - i];
            dig[len - 1 - i] = t;
        }

        /* Check if this digit pattern already exists in the SAM */
        int state = 0;
        bool exists = true;
        for (int i = 0; i < len; ++i) {
            int d = dig[i];
            if (sam_states[state].next[d] == -1) {
                exists = false;
                break;
            }
            state = sam_states[state].next[d];
        }

        if (!exists) {
            p_append((seq_t)cur); /* store the punctual bird number */
        }

        /* Extend SAM with the new digits */
        for (int i = 0; i < len; ++i) {
            sam_extend(dig[i]);
        }

        ++next_to_process;
    }

    return punctual[n - 1];
}
