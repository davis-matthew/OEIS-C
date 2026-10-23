/*
 * A034722  Products of successive Fibonacci numbers.
 * https://oeis.org/A034722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth smallest number that can be written as a product of
 *            successive Fibonacci numbers (of any length ≥ 1)
 * Range:     u8 n <= 8, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   add_checked
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_checked.h"

/* ------------------------------------------------------------------ */
/*  Internal helpers                                                      */
/* ------------------------------------------------------------------ */

typedef struct {
    seq_t value;         /* current product                           */
    idx_t start;         /* starting Fibonacci index (1‑based)         */
    idx_t next_idx;      /* next Fibonacci index to multiply by       */
} State;

/* Fibonacci cache */
static seq_t *fib_arr   = NULL;
static idx_t  fib_len   = 0;   /* number of Fibonacci numbers stored (≥2)  */
static idx_t  fib_cap   = 0;   /* allocated capacity                      */
static idx_t  next_start_seed = 1;   /* next start index that has not been seeded */

/* Sequence cache */
static seq_t *seq_arr   = NULL;
static idx_t  seq_len   = 0;   /* number of unique products stored         */
static idx_t  seq_cap   = 0;   /* allocated capacity                      */

/* Min‑heap for enumeration */
static State *heap_arr  = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

/* ------------------------------------------------------------------ */
/*  Utility functions                                                    */
/* ------------------------------------------------------------------ */

/* Grow sequence array to accommodate at least new_len elements */
static void grow_seq(idx_t new_len) {
    if (seq_cap >= new_len) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < (size_t)new_len) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = (idx_t)new_cap;
}

/* Push a state onto the min‑heap */
static void heap_push(State s) {
    if (heap_size + 1 > heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 4;
        heap_arr = (State *)realloc(heap_arr, new_cap * sizeof(State));
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap_arr[i] = s;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_arr[p].value <= heap_arr[i].value) break;
        State tmp = heap_arr[p];
        heap_arr[p] = heap_arr[i];
        heap_arr[i] = tmp;
        i = p;
    }
}

/* Pop the smallest state from the heap (heap is assumed non‑empty) */
static State heap_pop(void) {
    State top = heap_arr[0];
    heap_size--;
    if (heap_size > 0) heap_arr[0] = heap_arr[heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && heap_arr[l].value < heap_arr[smallest].value)
            smallest = l;
        if (r < heap_size && heap_arr[r].value < heap_arr[smallest].value)
            smallest = r;
        if (smallest == i) break;
        State tmp = heap_arr[i];
        heap_arr[i] = heap_arr[smallest];
        heap_arr[smallest] = tmp;
        i = smallest;
    }
    return top;
}

/* Checked multiplication: returns true if result fits, false otherwise.
   For 64‑bit seq_t, overflow is detected using unsigned __int128. */
static bool mul_checked(seq_t a, seq_t b, seq_t *res) {
    if (sizeof(seq_t) == 8) {
        unsigned __int128 tmp = (unsigned __int128)a * (unsigned __int128)b;
        if (tmp > (unsigned __int128)UINT64_MAX) return false;
        *res = (seq_t)tmp;
        return true;
    }
    *res = a * b;
    return true;
}

/* Ensure that fib_arr contains at least up to fib_index (1‑based). */
static bool ensure_fib(idx_t fib_index) {
    if (fib_len >= fib_index) return true;
    while (fib_len < fib_index) {
        seq_t a = fib_arr[fib_len];
        seq_t b = fib_arr[fib_len - 1];
        seq_t c;
        if (!add_checked(a, b, &c)) {
            /* Further Fibonacci numbers would overflow 64‑bit */
            return false;
        }
        if (fib_len + 1 >= fib_cap) {
            size_t new_cap = fib_cap ? fib_cap * 2 : 8;
            fib_arr = (seq_t *)realloc(fib_arr, new_cap * sizeof(seq_t));
            fib_cap = new_cap;
        }
        fib_arr[++fib_len] = c;
    }
    /* Seed any new starting indices that have not yet been added to the heap */
    while (next_start_seed <= fib_len) {
        State s = {fib_arr[next_start_seed], next_start_seed, next_start_seed + 1};
        heap_push(s);
        ++next_start_seed;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Initialization routine                                               */
/* ------------------------------------------------------------------ */

static bool initialized = false;

static void init(void) {
    if (initialized) return;

    /* Fibonacci cache: start with F1 = F2 = 1 */
    fib_cap  = 8;
    fib_arr  = (seq_t *)malloc(fib_cap * sizeof(seq_t));
    fib_arr[1] = (seq_t)1;
    fib_arr[2] = (seq_t)1;
    fib_len   = 2;
    next_start_seed = 1;

    /* Heap cache */
    heap_cap  = 64;
    heap_arr  = (State *)malloc(heap_cap * sizeof(State));

    /* Sequence cache */
    seq_cap   = 16;
    seq_arr   = (seq_t *)malloc(seq_cap * sizeof(seq_t));

    /* Seed starting indices 1 and 2 */
    while (next_start_seed <= fib_len) {
        State s = {fib_arr[next_start_seed], next_start_seed, next_start_seed + 1};
        heap_push(s);
        ++next_start_seed;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Sequence generation                                                   */
/* ------------------------------------------------------------------ */

/* Generate the sequence up to at least target terms. */
static void ensure_sequence(idx_t target) {
    if (!initialized) init();
    while (seq_len < target) {
        if (heap_size == 0) break;   /* no more producible values (overflowed) */

        State s = heap_pop();

        /* Record unique value */
        if (seq_len == 0 || s.value != seq_arr[seq_len - 1]) {
            grow_seq(seq_len + 1);
            seq_arr[seq_len++] = s.value;
        }

        /* Push next product for the same starting index, if it does not overflow */
        idx_t nxt = s.next_idx;
        if (!ensure_fib(nxt)) continue;        /* cannot compute further Fibonacci numbers */

        seq_t next_prod;
        if (mul_checked(s.value, fib_arr[nxt], &next_prod)) {
            State ns = {next_prod, s.start, nxt + 1};
            heap_push(ns);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Exported function                                                   */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined, but safe */
    ensure_sequence(n);
    if (n <= seq_len) return seq_arr[n - 1];
    /* If we do not have enough terms (overflow), return 0 (unreliable). */
    return (seq_t)0;
}
