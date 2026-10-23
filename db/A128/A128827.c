/*
 * A128827  Perfect powers beginning and ending with the same digit.
 * https://oeis.org/A128827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_small, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A128827 – Perfect powers beginning and ending with the same digit
    This routine maintains a min‑heap of perfect powers and a cache of
    all terms found so far.  It is lazy and works correctly for any
    order of calls.*/
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/seq_one.h"

typedef struct {
    seq_t val;          /* the perfect power value */
    unsigned int base;  /* base of the power */
    unsigned int exp;   /* current exponent (>=2) */
} node_t;

/* ---------- min‑heap helpers ------------------------------------------------ */

static void heap_push(node_t **heap, size_t *size, size_t *cap, node_t n)
{
    if (*size >= *cap) {
        size_t newcap = (*cap == 0) ? 2 : (*cap << 1);
        node_t *tmp = (node_t *)realloc(*heap, newcap * sizeof(node_t));
        *heap = tmp;
        *cap = newcap;
    }
    size_t i = *size;
    (*heap)[i] = n;
    ++(*size);
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*heap)[i].val < (*heap)[p].val) {
            node_t t = (*heap)[i];
            (*heap)[i] = (*heap)[p];
            (*heap)[p] = t;
            i = p;
        } else break;
    }
}

static node_t heap_pop(node_t **heap, size_t *size)
{
    node_t min = (*heap)[0];
    (*size)--;
    (*heap)[0] = (*heap)[*size];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        if (l >= *size) break;
        size_t r = l + 1;
        size_t s = l;
        if (r < *size && (*heap)[r].val < (*heap)[l].val) s = r;
        if ((*heap)[s].val < (*heap)[i].val) {
            node_t t = (*heap)[i];
            (*heap)[i] = (*heap)[s];
            (*heap)[s] = t;
            i = s;
        } else break;
    }
    return min;
}

/* ---------- sequence generator --------------------------------------------- */

seq_t a(idx_t n)
{
    /* static state */
    static node_t *heap = NULL;
    static size_t heap_size = 0, heap_cap = 0;
    static bool heap_inited = false;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static bool cache_inited = false;

    static seq_t last_val = 0;
    static bool have_last = false;

    /* base we start with for perfect powers (upper bound for base) */
    const unsigned int MAX_BASE = 100000U;   /* enough for n up to ~1e4 */

    /* initialise heap and cache once */
    if (!heap_inited) {
        heap_cap = MAX_BASE;  /* capacity for initial nodes */
        heap = (node_t *)malloc(heap_cap * sizeof(node_t));
        heap_size = 0;
        for (unsigned int b = 2U; b <= MAX_BASE; ++b) {
            node_t node;
            node.base = b;
            node.exp = 2U;
            node.val = mul_small((seq_t)b, b); /* b^2 */
            heap_push(&heap, &heap_size, &heap_cap, node);
        }
        heap_inited = true;
    }

    if (!cache_inited) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;                /* term 1 = 1 */
        cache[0] = seq_one();         /* first term */
        cache_inited = true;
    }

    /* special case: first term is 1 */
    if (n == 1U) return seq_one();

    /* generate terms until we reach desired index */
    while (cache_len < n) {
        node_t cur = heap_pop(&heap, &heap_size);
        seq_t current = cur.val;

        /* skip duplicate values */
        if (have_last && current == last_val) {
            /* nothing */
        } else {
            /* compute first and last digit of current */
            unsigned int last_digit = (unsigned int)(current % 10);
            seq_t temp = current;
            while (temp >= 10) temp /= 10;
            unsigned int first_digit = (unsigned int)temp;
            if (first_digit == last_digit) {
                /* store the new term */
                if (cache_len == (idx_t)cache_cap) {
                    size_t newcap = cache_cap ? cache_cap << 1 : 32;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = newcap;
                }
                cache[cache_len++] = current;
            }
            last_val = current;
            have_last = true;
        }

        /* push next power of the same base */
        node_t next;
        next.base = cur.base;
        next.exp = cur.exp + 1U;
        next.val = mul_small(current, cur.base); /* current * base : b^(exp+1) */
        heap_push(&heap, &heap_size, &heap_cap, next);
    }

    return cache[n-1];
}
