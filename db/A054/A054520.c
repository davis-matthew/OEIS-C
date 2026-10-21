/*
 * A054520  Let S = {1,5,9,13,..., 4n+1, ...} and call p in S an S-prime if
 *          p>1 and the only divisors of p in S are 1 and p; sequence gives
 *          elements of S that are not S-primes.
 * https://oeis.org/A054520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4069, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   f_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A054520:  numbers ≡1 (mod 4) that are NOT S‑primes
 *
 * S = {1,5,9,13,…}   (numbers 1 (mod 4))
 * An S‑prime is a number >1 whose only divisors in S are 1 and itself.
 * A number that is not an S‑prime is of the form
 *     4·(m + n + 4mn) + 1   with m,n > 0
 * because     (4m+1)(4n+1) = 4·(m + n + 4mn) + 1.
 *
 * The function builds the sequence on demand by generating all numbers of
 * that form via a min‑heap, discarding duplicates, and caching the
 * resulting sorted list.  The first term is 1 (treated specially).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/f_val.h"

/* heap entry ------------------------------------------------------------- */
typedef struct {
    seq_t  val;   /* value of 4·(m + n + 4mn) + 1   */
    uint64_t m;
    uint64_t n;
} heap_entry;

/* static state ----------------------------------------------------------- */
static seq_t *cache     = NULL;   /* finished sequence terms     */
static idx_t  cache_sz  = 0;      /* number of terms in cache    */
static size_t cache_cap = 0;      /* cache capacity              */

static heap_entry *heap   = NULL; /* min‑heap of candidates      */
static size_t     heap_sz = 0;    /* number of elements in heap  */
static size_t     heap_cap= 0;    /* heap capacity               */

static uint64_t   max_m_used  = 0; /* largest m inserted so far   */
static seq_t      last_cached_val; /* last value stored          */

/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* push an entry onto the binary min‑heap                                  */
static void heap_push(heap_entry e)
{
    if (heap_sz + 1 >= heap_cap) {            /* grow heap if full      */
        size_t new_cap = heap_cap ? heap_cap * 2 : 64;
        heap_entry *tmp = (heap_entry *)realloc(heap, new_cap * sizeof(heap_entry));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = ++heap_sz;                     /* insert at bottom      */
    while (i > 1) {
        size_t parent = i >> 1;
        if (heap[parent].val <= e.val) break;
        heap[i] = heap[parent];               /* bubble up              */
        i = parent;
    }
    heap[i] = e;
}

/* ------------------------------------------------------------------------ */
/* pop the smallest entry from the heap                                    */
static heap_entry heap_pop(void)
{
    heap_entry top = heap[1];
    heap[1] = heap[heap_sz--];                /* move last to root     */
    size_t i = 1;
    while (true) {
        size_t left = i << 1;
        if (left > heap_sz) break;            /* no children           */
        size_t right = left + 1;
        size_t child = (right <= heap_sz && heap[right].val < heap[left].val) ? right : left;
        if (heap[child].val >= heap[i].val) break;
        heap_entry tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }
    return top;
}

/* ------------------------------------------------------------------------ */
/* ensure that the cache contains at least `target` terms                  */
static void ensure_cache(idx_t target)
{
    if (cache_sz >= target) return;          /* already ready          */

    /* --- initialise cache with 1 ------------------------------------- */
    if (cache_sz == 0) {
        if (!cache) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();
        }
        cache[0] = (seq_t)1;
        cache_sz  = 1;
        last_cached_val = (seq_t)1;
    }

    /* --- insert new m values if needed --------------------------------- */
    uint64_t need_m = target + 10;           /* generous bound          */
    if (need_m > max_m_used) {
        for (uint64_t m = max_m_used + 1; m <= need_m; ++m) {
            heap_entry e;
            e.m = m;  e.n = 1;
            e.val = f_val(m, 1);
            heap_push(e);
        }
        max_m_used = need_m;
    }

    /* --- pop from heap until enough terms are cached ------------------- */
    while (cache_sz < target) {
        heap_entry e = heap_pop();           /* smallest candidate     */
        seq_t val = e.val;

        /* skip duplicates */
        if (!(cache_sz > 0 && val == last_cached_val)) {
            if (cache_sz >= cache_cap) {     /* grow cache when full   */
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = val;
            last_cached_val = val;
        }

        /* push the next candidate for this m */
        heap_entry next;
        next.m = e.m;
        next.n = e.n + 1;
        next.val = f_val(e.m, next.n);
        heap_push(next);
    }
}

/* ------------------------------------------------------------------------ */
/* public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range – undefined */
    ensure_cache(n);
    return cache[n - 1];
}
