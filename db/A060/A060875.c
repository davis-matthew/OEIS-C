/*
 * A060875  Intrinsic 5-palindromes: n is an intrinsic k-palindrome if it is a
 *          k-digit palindrome in some base.
 * https://oeis.org/A060875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest integer that is a 5‑digit palindrome ABBBBA
 *            in some base b ≥ 2 (digits a∈[1,b‑1], d∈[0,b‑1], c∈[0,b‑1])
 * Range:     u8 n <= 5, u16 n <= 2994, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_val, next_triple, pow4plus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_val.h"
#include "../utils/next_triple.h"
#include "../utils/pow4plus1.h"

typedef struct {
    seq_t val; /* palindrome value */
    idx_t base;
    uint32_t a, d, c; /* digit triple */
} Node;

/* ---------- heap for next candidates ---------- */
static Node *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_sz  = 0;

static void heap_expand(void)
{
    size_t new_cap = heap_cap ? heap_cap << 1 : 16;
    Node *tmp = (Node *)realloc(heap, new_cap * sizeof(Node));
    if (!tmp) exit(EXIT_FAILURE);
    heap = tmp;
    heap_cap = new_cap;
}

static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        Node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static void heap_sift_down(size_t i)
{
    while (true) {
        size_t l = (i << 1) + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < heap_sz && heap[r].val < heap[l].val) smallest = r;
        if (heap[i].val <= heap[smallest].val) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void heap_push(Node n)
{
    if (heap_sz == heap_cap) heap_expand();
    heap[heap_sz++] = n;
    heap_sift_up(heap_sz - 1);
}

static Node heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_sz];
    heap_sift_down(0);
    return top;
}

/* ---------- cache of sequence ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;

static void cache_append(seq_t v)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = v;
}

/* ---------- main API ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* <offset> irrelevant */

    /* expand cache until we have enough terms */
    static idx_t next_base = 2; /* next base to consider for inclusion */

    while (cache_sz < n) {
        /* ensure at least one candidate exists */
        if (heap_sz == 0) {
            Node init = { .base = next_base, .a = 1, .d = 0, .c = 0 };
            init.val = calc_val(init.base, init.a, init.d, init.c);
            heap_push(init);
        }

        Node cur = heap_pop();

        /* deduplicate */
        if (cache_sz == 0 || cache[cache_sz-1] != cur.val)
            cache_append(cur.val);

        /* push next triple from the same base if available */
        if (next_triple(cur.base, &cur.a, &cur.d, &cur.c)) {
            cur.val = calc_val(cur.base, cur.a, cur.d, cur.c);
            heap_push(cur);
        }

        /* add new bases whose minimal value is <= cur.val */
        while (true) {
            seq_t min_next = pow4plus1(next_base);
            if (min_next <= cur.val) {
                Node nb = { .base = next_base, .a = 1, .d = 0, .c = 0 };
                nb.val = calc_val(nb.base, nb.a, nb.d, nb.c);
                heap_push(nb);
                ++next_base;
            } else break;
        }
    }

    return cache[n-1];
}
