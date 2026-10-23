/*
 * A102358  Finite sequence of iterations at which Langton's Ant passes
 *          through the origin.
 * https://oeis.org/A102358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   a(n) = step number of the nth visit of Langton's Ant to the
 *            origin (a(1) = 0). The function simulates the ant step by step
 *            and records the step numbers at which the origin is visited
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   hash_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/hash_int64.h"

/* If the compiler is C++, cast malloc/calloc to the required pointer type. */
#ifndef VALGRIND
#define ALLOC(type, size) ((type)malloc((size)))
#define CALLOC(type, nmemb, size) ((type)calloc((nmemb), (size)))
#else
/* In Valgrind builds, the cast may be unneeded. */
#define ALLOC(type, size) malloc((size))
#define CALLOC(type, nmemb, size) calloc((nmemb), (size))
#endif

/* ---------- Settings for the hash table ---------- */
static const size_t HASH_CAP = 1 << 20;      /* 1,048,576 entries (≈18 MB) */
static const size_t HASH_MASK = HASH_CAP - 1;

/* ---------- Static storage ---------- */
static int64_t *kx = NULL;
static int64_t *ky = NULL;
static unsigned char *used = NULL;
static unsigned char *color = NULL;
static bool hash_inited = false;

static seq_t *visits = NULL;
static idx_t visits_cap = 0;
static idx_t visits_size = 0;

/* ---------- Helper functions ---------- */

/* Initialise the hash table once. */
static void init_hash_table(void) {
    if (hash_inited) return;
    kx = ALLOC(int64_t *, HASH_CAP * sizeof(int64_t));
    ky = ALLOC(int64_t *, HASH_CAP * sizeof(int64_t));
    used = CALLOC(unsigned char *, HASH_CAP, 1);
    color = CALLOC(unsigned char *, HASH_CAP, 1);
    hash_inited = true;
}

/* Find the slot for (x,y).  Set *found to true if the entry exists. */
static size_t find_slot(int64_t x, int64_t y, bool *found) {
    uint64_t h = hash_int64(x) ^ (hash_int64(y) << 1);
    size_t pos = h & HASH_MASK;
    for (;;) {
        if (!used[pos]) {
            *found = false;
            return pos;
        }
        if (kx[pos] == x && ky[pos] == y) {
            *found = true;
            return pos;
        }
        pos = (pos + 1) & HASH_MASK;
    }
}

/* Insert a new black cell or toggle its color if it already exists. */
static void insert_or_toggle(int64_t x, int64_t y, unsigned char current_color) {
    bool found;
    size_t pos = find_slot(x, y, &found);
    if (found) {
        color[pos] ^= 1;          /* flip 0↔1 */
    } else {
        used[pos] = 1;
        kx[pos] = x;
        ky[pos] = y;
        color[pos] = current_color ^ 1;   /* new color after toggle */
    }
}

/* Ensure the visits array can hold new entries. */
static void ensure_visits_cap(void) {
    if (visits_size >= visits_cap) {
        idx_t new_cap = visits_cap == 0 ? 64 : visits_cap << 1;
        seq_t *new_visits = (seq_t *)realloc(visits, new_cap * sizeof(seq_t));
        visits = new_visits;
        visits_cap = new_cap;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;     /* undefined, but safe */

    if (!hash_inited) init_hash_table();

    /* Initialise visits array on first call */
    if (visits_size == 0) {
        visits_cap = 64;
        visits = (seq_t *)malloc(visits_cap * sizeof(seq_t));
        visits_size = 1;          /* first visit at step 0 */
        visits[1] = (seq_t)0;
    }

    /* Simulate until we have recorded n visits to the origin */
    while (visits_size < n) {
        static int64_t x = 0, y = 0;
        static int dir = 0;      /* 0=N,1=E,2=S,3=W */
        static idx_t step = 0;

        /* 1. Look up current cell colour */
        bool found;
        size_t pos = find_slot(x, y, &found);
        unsigned char col = found ? color[pos] : 0;

        /* 2. Turn */
        dir = (col == 0) ? ((dir + 1) & 3) : ((dir + 3) & 3);

        /* 3. Flip colour */
        insert_or_toggle(x, y, col);

        /* 4. Move one step */
        switch (dir) {
            case 0: ++y; break;
            case 1: ++x; break;
            case 2: --y; break;
            case 3: --x; break;
        }
        ++step;

        /* 5. Check if we are back at the origin */
        if (x == 0 && y == 0) {
            ++visits_size;
            ensure_visits_cap();
            visits[visits_size] = (seq_t)step;
        }
    }

    return visits[n];
}
