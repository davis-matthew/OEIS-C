/*
 * A099225  Numbers of the form m^k+k, with m and k > 1.
 * https://oeis.org/A099225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 332, u32 n <= 2384, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A099225: numbers of the form m^k + k, with m,k>1, sorted
    ascending and without duplicates.  This implementation generates
    terms lazily and caches them so that a(n) can be requested in any
    order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

/* data structure for the heap elements */
typedef struct {
    seq_t  val;   /* current value m^k + k   */
    uint64_t m;   /* base    */
    uint32_t k;   /* exponent */
} node_t;

/* ---------- min‑heap of node_t ---------- */
static node_t *heap      = NULL;
static size_t  heap_cap  = 0;
static size_t  heap_len  = 0;

static void heap_push(node_t *n)
{
    /* grow the heap if necessary */
    if (heap_len >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 4;
        node_t *tmp = (node_t *)realloc(heap, new_cap * sizeof(node_t));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_len++;
    heap[i] = *n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[i].val < heap[p].val) {
            node_t tmp = heap[i];
            heap[i] = heap[p];
            heap[p] = tmp;
            i = p;
        } else {
            break;
        }
    }
}

static void heap_pop(node_t *out)
{
    *out = heap[0];
    heap_len--;
    if (heap_len == 0) return;
    heap[0] = heap[heap_len];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        if (l >= heap_len) break;
        size_t c = l;
        if (r < heap_len && heap[r].val < heap[l].val) c = r;
        if (heap[c].val < heap[i].val) {
            node_t tmp = heap[i];
            heap[i] = heap[c];
            heap[c] = tmp;
            i = c;
        } else {
            break;
        }
    }
}

/* ---------- memoised cache of sequence values ---------- */
static seq_t *cache      = NULL;
static size_t cache_cap   = 0;
static size_t cache_len   = 0;

/* ---------- state for incremental generation ---------- */
static uint64_t next_base   = 3;   /* next base to be inserted into the heap */
static bool     has_prev    = false;
static seq_t   prev_val;           /* previous value produced (for duplicate check) */

/* ---------- one‑time initialisation ---------- */
static void init_sequence(void)
{
    node_t n;
    n.m = 2;
    n.k = 2;
    n.val = big_pow((seq_t)2, (seq_t)2) + (seq_t)2;
    heap_push(&n);
    next_base = 3;
    has_prev = false;
    /* cache stays empty until first element is generated */
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but harmless */

    /* one‑time initialisation */
    if (!cache) {
        init_sequence();
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {

        /* insert new bases whose initial candidate <= current minimum */
        while (heap_len > 0) {
            seq_t next_init = big_pow((seq_t)next_base, (seq_t)2) + (seq_t)2;
            if (next_init <= heap[0].val) {
                node_t nm;
                nm.m = next_base;
                nm.k = 2;
                nm.val = next_init;
                heap_push(&nm);
                next_base++;
            } else {
                break;
            }
        }

        /* pop the smallest candidate */
        node_t cur;
        heap_pop(&cur);

        /* check for duplicate value */
        bool duplicate = false;
        if (has_prev && cur.val == prev_val) {
            duplicate = true;
        } else {
            has_prev = true;
            prev_val = cur.val;
        }

        /* record the unique term */
        if (!duplicate) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur.val;
        }

        /* push the next candidate for this base */
        cur.k++;
        cur.val = big_pow((seq_t)cur.m, (seq_t)cur.k) + (seq_t)cur.k;
        heap_push(&cur);
    }

    return cache[n - 1];
}
