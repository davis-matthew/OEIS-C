/*
 * A190784  Numbers whose binary representation is a substring of the
 *          concatenation of the binary representation of all smaller
 *          nonnegative integers not listed earlier, taken in decreasing
 *          order.
 * https://oeis.org/A190784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A190784: Numbers whose binary representation occurs as a substring of
 * the concatenation of the binary representations of all smaller
 * nonnegative integers not listed earlier, taken in decreasing order.
 * The function builds a suffix automaton of the reversed binary strings
 * of the unselected numbers and tests each candidate against it. */

/* These types are provided by the build environment.
 * seq_t   – the integer type used for sequence values (uint8_t..__int128
 *          or an arbitrary-precision C++ integer class).
 * idx_t   – the index type, always unsigned 64‑bit (uint64_t). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Suffix automaton for binary strings ---------- */
typedef struct {
    int next[2];
    int link;
    int len;
} State;

/* Global SA state */
static State *sa_states = NULL;    /* array of states */
static int   sa_size   = 0;        /* number of states */
static int   sa_cap    = 0;        /* capacity of sa_states array */
static int   sa_last   = 0;        /* last state (corresponds to whole string) */

/* Ensure SA has capacity for at least 'need' states */
static void ensure_sa_cap(int need)
{
    if (need > sa_cap) {
        int newcap = sa_cap ? sa_cap * 2 : 1024;
        while (need > newcap) newcap *= 2;
        State *tmp = (State*)realloc(sa_states, newcap * sizeof(State));
        if (!tmp) abort();
        sa_states = tmp;
        sa_cap = newcap;
    }
}

/* Extend SA with one bit (0 or 1) of the reversed string */
static void sa_extend_bit(int c)
{
    ensure_sa_cap(sa_size + 1);
    int cur = sa_size++;
    sa_states[cur].len  = sa_states[sa_last].len + 1;
    sa_states[cur].link = 0;
    sa_states[cur].next[0] = sa_states[cur].next[1] = -1;

    int p = sa_last;
    while (p != -1 && sa_states[p].next[c] == -1) {
        sa_states[p].next[c] = cur;
        p = sa_states[p].link;
    }
    if (p == -1) {
        sa_states[cur].link = 0;
    } else {
        int q = sa_states[p].next[c];
        if (sa_states[p].len + 1 == sa_states[q].len) {
            sa_states[cur].link = q;
        } else {
            ensure_sa_cap(sa_size + 1);
            int clone = sa_size++;
            sa_states[clone].len  = sa_states[p].len + 1;
            sa_states[clone].link = sa_states[q].link;
            sa_states[clone].next[0] = sa_states[q].next[0];
            sa_states[clone].next[1] = sa_states[q].next[1];
            while (p != -1 && sa_states[p].next[c] == q) {
                sa_states[p].next[c] = clone;
                p = sa_states[p].link;
            }
            sa_states[q].link = sa_states[cur].link = clone;
        }
    }
    sa_last = cur;
}

/* Add reversed binary representation of an integer x to the SA */
static void add_reversed_bits(idx_t x)
{
    if (x == 0) {
        sa_extend_bit(0);
        return;
    }
    while (x > 0) {
        sa_extend_bit((int)(x & 1));
        x >>= 1;
    }
}

/* Test whether binary representation of x occurs as a substring of
 * the current concatenation. Since SA stores reversed strings, we
 * query with bits reversed (LSB first). */
static bool pattern_in_sam(idx_t x)
{
    int state = 0;
    if (x == 0) {
        int c = 0;
        int nxt = sa_states[state].next[c];
        return nxt != -1;
    }

    int bits[64];
    int len = 0;
    idx_t y = x;
    while (y > 0) {
        bits[len++] = y & 1;
        y >>= 1;
    }
    /* Bits are LSB first – query in this order to match the reversed
       SA. */
    for (int i = 0; i < len; ++i) {
        int c = bits[i];
        int nxt = sa_states[state].next[c];
        if (nxt == -1) return false;
        state = nxt;
    }
    return true;
}

/* ---------- Data structures for selected numbers and sequence ---------- */
static unsigned char *selected = NULL;  /* 0 = not yet selected, 1 = selected */
static size_t sel_cap = 0;

static void ensure_selected(idx_t i)
{
    if ((size_t)i >= sel_cap) {
        size_t old = sel_cap;
        size_t newcap = old ? old * 2 : 128;
        while ((size_t)i >= newcap) newcap *= 2;
        unsigned char *tmp = (unsigned char*)realloc(selected, newcap * sizeof(unsigned char));
        if (!tmp) abort();
        if (newcap > old) memset(tmp + old, 0, newcap - old);
        selected = tmp;
        sel_cap = newcap;
    }
}

static seq_t *seq_arr = NULL;   /* array of sequence values */
static size_t seq_cap = 0, seq_len = 0;

/* Current counter: last number that has been processed (either added or
 * tested). */
static idx_t current = 0;

/* Initialization flag */
static bool initialized = false;

/* ---------- The sequence generator ---------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* init structures */
        ensure_selected(0);
        selected[0] = 0;

        sa_cap   = 1024;
        sa_states = (State*)malloc(sa_cap * sizeof(State));
        if (!sa_states) abort();
        sa_states[0].len  = 0;
        sa_states[0].link = -1;
        sa_states[0].next[0] = sa_states[0].next[1] = -1;
        sa_size = 1;
        sa_last = 0;

        seq_cap = 16;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();
        seq_len = 0;
        current = 0;

        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Add the current number to the SA if it is not yet selected */
        ensure_selected(current);
        if (!selected[current])
            add_reversed_bits(current);

        idx_t candidate = current + 1;
        ensure_selected(candidate);

        if (pattern_in_sam(candidate)) {
            /* Mark candidate as selected and store in the sequence */
            if (!selected[candidate])
                selected[candidate] = 1;
            if (seq_len >= seq_cap) {
                size_t newcap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(seq_arr, newcap * sizeof(seq_t));
                if (!tmp) abort();
                seq_arr = tmp;
                seq_cap = newcap;
            }
            seq_arr[seq_len++] = (seq_t)candidate;
        }
        current = candidate;
    }

    return seq_arr[n - 1];
}
