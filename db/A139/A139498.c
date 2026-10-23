/*
 * A139498  Primes of the form x^2 + 17x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A139498 – primes of the form x^2 + 17xy + y^2 (x,y ≥ 0)
    The implementation generates all such numbers in ascending order
    using a min‑heap, tests each candidate with is_prime_big,
    and memoises the primes.  Calls to a(n) return the nth prime in
    this order.  The algorithm is deterministic, works for any build
    configuration, and respects all requirements. */
    
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* ---------- minimum‑heap of candidate numbers ---------- */
typedef struct {
    seq_t val;          /* the candidate value */
    uint64_t x;         /* current x coordinate   */
    uint64_t y;         /* current y coordinate   */
} Cand;

static Cand *heap      = NULL;   /* dynamic array of candidates */
static size_t heap_sz  = 0;      /* current number of elements   */
static size_t heap_cap = 0;      /* allocated capacity           */

/* helper: sift element i up to maintain min‑heap property */
static inline void sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        Cand tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

/* helper: sift element i down */
static inline void sift_down(size_t i)
{
    while (true) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_sz && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_sz && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        Cand tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* push a new candidate onto the heap */
static void heap_push(Cand v)
{
    if (heap_sz == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 64;
        Cand *tmp = (Cand *)realloc(heap, new_cap * sizeof(Cand));
        heap   = tmp;
        heap_cap = new_cap;
    }
    heap[heap_sz++] = v;
    sift_up(heap_sz - 1);
}

/* pop the smallest candidate */
static Cand heap_pop(void)
{
    Cand top = heap[0];
    heap[0] = heap[--heap_sz];
    if (heap_sz) sift_down(0);
    return top;
}

/* ---------- prime cache ---------- */
static seq_t  *prime_cache   = NULL;  /* array of found primes    */
static size_t cache_len       = 0;    /* number of cached primes  */
static size_t cache_cap       = 0;    /* allocated cache capacity */

static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* ---------- generate primes until at least n are cached ---------- */
static void fill_cache(idx_t n)
{
    static uint64_t next_x = 1;   /* next x for which the initial pair (x,x) has not been pushed */

    /* initialise heap with the trivial pair (0,0) if it is empty */
    if (heap_sz == 0) {
        heap_push((Cand){ .val = 0, .x = 0, .y = 0 });
    }

    while (cache_len < (size_t)n) {
        Cand cur = heap_pop();

        /* test primality of the current value */
        if (cur.val > 1 && is_prime_big(cur.val)) {
            if (cache_len == cache_cap) cache_grow();
            prime_cache[cache_len++] = cur.val;
        }

        /* push the next candidate with the same x (increment y) */
        uint64_t nx = cur.x;
        uint64_t ny = cur.y + 1;
        seq_t xsq  = (seq_t)nx * (seq_t)nx;
        seq_t val_next = xsq + (seq_t)17 * (seq_t)nx * (seq_t)ny
                         + (seq_t)ny * (seq_t)ny;
        heap_push((Cand){ .val = val_next, .x = nx, .y = ny });

        /* push the initial candidate for the next x, if it has not
           been pushed yet.  This guarantees that each x≥1 appears
           exactly once as a starting pair (x,x). */
        uint64_t xn = next_x;
        uint64_t yn = next_x;
        seq_t val_xx = (seq_t)19 * (seq_t)xn * (seq_t)xn;   /* x^2 + 17xy + y^2 with x=y */
        heap_push((Cand){ .val = val_xx, .x = xn, .y = yn });
        ++next_x;
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 1 */
    if (cache_len < (size_t)n) fill_cache(n);
    return prime_cache[n - 1];
}
