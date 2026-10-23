/*
 * A020692  Numbers of form 7 x^2 + 10 y^2.
 * https://oeis.org/A020692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   pair_hash_key, seq_type_has_limit, seq_type_max_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of form 7*x^2 + 10*y^2, produced in increasing order.
   a(n) returns the nth such number (n ≥ 1). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pair_hash_key.h"
#include "../utils/seq_type_has_limit.h"
#include "../utils/seq_type_max_ll.h"

/* ----------  min‑heap for Candidate values  ---------- */
typedef struct { seq_t val; idx_t x; idx_t y; } Candidate;
static Candidate *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;

static void heap_reserve(size_t new_cap)
{
    if (new_cap <= heap_cap) return;
    heap_cap = new_cap;
    heap = (Candidate *)realloc(heap, heap_cap * sizeof(Candidate));
    if (!heap) abort();
}

static void heap_push(Candidate c)
{
    heap_reserve(heap_size + 1);
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= c.val) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = c;
}

static Candidate heap_pop(void)
{
    if (heap_size == 0) abort();
    Candidate top = heap[0];
    heap_size--;
    if (heap_size) {
        Candidate last = heap[heap_size];
        size_t i = 0;
        while (1) {
            size_t left = (i << 1) + 1;
            if (left >= heap_size) break;
            size_t right = left + 1;
            size_t child = (right < heap_size && heap[right].val < heap[left].val) ? right : left;
            if (last.val <= heap[child].val) break;
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return top;
}

/* ----------  hash set of visited (x,y) pairs  ---------- */
#define PAIR_SET_CAP_INIT (1 << 19)      /* 524,288 slots */

static uint64_t *pair_set = NULL;
static size_t pair_set_cap = 0;

static void pair_set_init(void)
{
    if (pair_set) return;
    pair_set_cap = PAIR_SET_CAP_INIT;
    pair_set = (uint64_t *)calloc(pair_set_cap, sizeof(uint64_t));
    if (!pair_set) abort();
}

static bool pair_set_insert(uint64_t key)
{
    pair_set_init();
    size_t idx = key & (pair_set_cap - 1);
    while (pair_set[idx] != 0) {
        if (pair_set[idx] == key) return false;     /* already present */
        idx = (idx + 1) & (pair_set_cap - 1);
    }
    pair_set[idx] = key;
    return true;                                   /* newly inserted */
}

/* ----------  storage for generated sequence  ---------- */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0, seq_cap = 0;

static void seq_reserve(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    seq_cap = new_cap;
    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    if (!seq_arr) abort();
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    /* initialise static storage on first call */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        seq_cap = 256;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();

        /* seed heap with (0,0) */
        Candidate c;
        c.val = (seq_t)0;
        c.x = c.y = 0;
        heap_push(c);
        pair_set_insert(0);
    }

    if (n == 0) return 0; /* undefined but harmless */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        Candidate cur = heap_pop();

        /* append if not duplicate of last value */
        if (seq_len == 0 || seq_arr[seq_len - 1] != cur.val)
            seq_arr[seq_len++] = cur.val;

        /* children (x+1,y) and (x,y+1) */
        idx_t nx, ny;
        uint64_t key;

        /* child (x+1, y) */
        nx = cur.x + 1;
        ny = cur.y;
        key = pair_hash_key(nx, ny);
        if (pair_set_insert(key)) {
            unsigned long long val_calc = 7ull * nx * nx + 10ull * ny * ny;
            if (!(seq_type_has_limit() && val_calc > seq_type_max_ll())) {
                Candidate c1;
                c1.val = (seq_t)val_calc;
                c1.x = nx; c1.y = ny;
                heap_push(c1);
            }
        }

        /* child (x, y+1) */
        nx = cur.x;
        ny = cur.y + 1;
        key = pair_hash_key(nx, ny);
        if (pair_set_insert(key)) {
            unsigned long long val_calc = 7ull * nx * nx + 10ull * ny * ny;
            if (!(seq_type_has_limit() && val_calc > seq_type_max_ll())) {
                Candidate c2;
                c2.val = (seq_t)val_calc;
                c2.x = nx; c2.y = ny;
                heap_push(c2);
            }
        }
    }

    return seq_arr[n - 1];
}
