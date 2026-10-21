/*
 * A217849  Numbers which are the sums of consecutive ninth powers.
 * https://oeis.org/A217849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest non‑negative integer that can be written
 *            as a sum of consecutive ninth powers. (a(1) = 0, a(2) = 1, … )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   max_start_64bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/max_start_64bit.h"

/* ---------- helper structures ---------- */

typedef struct {
    seq_t sum;   /* current sum of the sequence */
    idx_t s;     /* starting integer of the sequence */
    idx_t l;     /* number of terms already summed */
} heapnode;

/* ---------- heap implementation (min‑heap) ---------- */

#define HEAP_MAX 50000
static heapnode heap[HEAP_MAX];
static size_t heap_size = 0;

static void push_heap(seq_t sum, idx_t s, idx_t l)
{
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (sum >= heap[p].sum) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i].sum = sum;
    heap[i].s   = s;
    heap[i].l   = l;
}

static heapnode pop_heap(void)
{
    heapnode top = heap[0];
    heapnode last = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = (right < heap_size && heap[right].sum < heap[left].sum) ?
                       right : left;
        if (heap[child].sum >= last.sum) break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return top;
}

/* ---------- power of ninth ---------- */

static seq_t pow9(seq_t base)
{
    seq_t res = 1;
    for (int i = 0; i < 9; ++i) res *= base;
    return res;
}

/* ---------- cache of computed a(n) values ---------- */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* heap / start bookkeeping */
static idx_t next_start = 2;
static idx_t max_start   = 0;   /* 0 means no bound (for big‑int builds) */

/* initialisation flag */
static bool built = false;

/* Build the initial state */
static void init(void)
{
    if (built) return;
    built = true;

    /* initialise cache storage */
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache[0] = 0;            /* a(1) = 0 */
    cache_len = 1;

    /* initialise heap with the first sequence (start = 1, length = 1) */
    push_heap(pow9((seq_t)1), (idx_t)1, (idx_t)1);
    next_start = 2;

    /* compute max_start for 64‑bit seq_t */
    if (sizeof(seq_t) == 8) {
        max_start = max_start_64bit();
    } else {
        max_start = 0;   /* no bound for larger integer types */
    }
}

/* Ensure cache contains at least n terms (n is 1‑based) */
static void ensure_cache(idx_t n)
{
    init();
    while (cache_len < n) {
        /* extract the smallest available sum */
        heapnode node = pop_heap();
        seq_t value = node.sum;

        /* store value if not a duplicate of the previous one */
        if (cache_len == 0 || value != cache[cache_len - 1]) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(EXIT_FAILURE);
            }
            cache[cache_len++] = value;
        }

        /* generate next sum in this sequence */
        seq_t next_power = pow9((seq_t)(node.s + node.l));
        seq_t next_sum   = node.sum + next_power;
        if (next_sum >= node.sum) {   /* no wrap‑around */
            push_heap(next_sum, node.s, node.l + 1);
        }

        /* add a new starting sequence if we haven't exceeded the bound */
        if (max_start == 0 || next_start <= max_start) {
            seq_t start_power = pow9((seq_t)next_start);
            /* For 64‑bit builds, start_power still fits by construction of max_start */
            push_heap(start_power, next_start, (idx_t)1);
        }
        next_start++;
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
