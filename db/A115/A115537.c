/*
 * A115537  Numbers k such that the concatenation of 4*k with k gives a
 *          square.
 * https://oeis.org/A115537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 8, u128 n <= 8, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the concatenation of 4*k with k gives a square. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Prime sieve up to a fixed limit, used only by square_free_part. */
#define PRIME_LIMIT 10000
static bool primes_init = false;
static int primes[PRIME_LIMIT + 1];
static int prime_cnt = 0;

static void init_primes(void) {
    if (primes_init) return;
    for (int i = 0; i <= PRIME_LIMIT; ++i) primes[i] = 1;
    primes[0] = primes[1] = 0;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (primes[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                primes[j] = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (primes[i]) primes[prime_cnt++] = i;
    primes_init = true;
}

/* Return the square‑free part of x, removing all squares of primes. */
static seq_t square_free_part(seq_t x) {
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        seq_t p2 = (seq_t)p * (seq_t)p;
        while (x % p2 == 0) x /= p2;
    }
    return x;
}

/* Heap node holding the current k = s * u^2 and the index of its data set. */
typedef struct { seq_t val; int d; } HeapNode;

/* Simple binary min‑heap for HeapNode. */
typedef struct {
    HeapNode *data;
    size_t cap;
    size_t size;
} Heap;

static void heap_init(Heap *h) {
    h->cap = 4;
    h->size = 0;
    h->data = (HeapNode *)malloc(h->cap * sizeof(HeapNode));
}

static void heap_push(Heap *h, HeapNode n) {
    if (h->size == h->cap) {
        h->cap <<= 1;
        h->data = (HeapNode *)realloc(h->data, h->cap * sizeof(HeapNode));
    }
    size_t i = h->size++;
    h->data[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[i].val < h->data[p].val) {
            HeapNode t = h->data[i];
            h->data[i] = h->data[p];
            h->data[p] = t;
            i = p;
        } else break;
    }
}

static HeapNode heap_pop(Heap *h) {
    HeapNode ret = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < h->size && h->data[l].val < h->data[smallest].val)
            smallest = l;
        if (r < h->size && h->data[r].val < h->data[smallest].val)
            smallest = r;
        if (smallest != i) {
            HeapNode t = h->data[i];
            h->data[i] = h->data[smallest];
            h->data[smallest] = t;
            i = smallest;
        } else break;
    }
    return ret;
}

/* Data for one digit length d. */
typedef struct {
    seq_t s;       /* square‑free part of 4·10^d + 1 */
    seq_t u_cur;   /* current u for this d */
    seq_t u_high;  /* largest u allowed for this d */
} DData;

/* Main sequence function */
seq_t a(idx_t n) {
    /* Cache for already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Per‑digit‑length state. */
    static DData *ddata = NULL;
    static size_t ddata_cap = 0;
    static size_t ddata_len = 0;

    /* Priority queue for candidate k = s·u². */
    static Heap heap;
    static bool heap_inited = false;

    /* Loop counter for new digit lengths. */
    static unsigned int next_d = 0;   /* digit length processed so far (0‑based) */
    static seq_t pow10 = 1;           /* 10^d */

    if (!heap_inited) {
        heap_init(&heap);
        heap_inited = true;
        init_primes();
    }

    /* Produce enough terms to answer the current query. */
    while (cache_len < n) {
        /* If no candidates left, generate the next digit length. */
        while (heap.size == 0) {
            ++next_d;                       /* move to d = next_d (1‑based) */
            pow10 *= (seq_t)10;             /* pow10 = 10^d */

            /* Compute m = 4·10^d + 1 and its square‑free part s. */
            seq_t m = (seq_t)4 * pow10 + (seq_t)1;
            seq_t s = square_free_part(m);

            /* Lower and upper bounds for u². */
            seq_t ten_d_minus1 = pow10 / (seq_t)10;
            seq_t lower = (ten_d_minus1 + s - (seq_t)1) / s;
            seq_t u_low = floor_sqrt(lower);
            if (u_low * u_low < lower) u_low += 1;   /* ceil sqrt */

            seq_t upper = (pow10 - (seq_t)1) / s;
            seq_t u_high = floor_sqrt(upper);

            if (u_low <= u_high) {
                /* Ensure enough space in the ddata array. */
                if (ddata_len == ddata_cap) {
                    ddata_cap = ddata_cap ? ddata_cap << 1 : 4;
                    ddata = (DData *)realloc(ddata, ddata_cap * sizeof(DData));
                }
                ddata[ddata_len].s = s;
                ddata[ddata_len].u_cur = u_low;
                ddata[ddata_len].u_high = u_high;

                seq_t val = s * u_low * u_low;
                heap_push(&heap, (HeapNode){ val, (int)ddata_len });

                ++ddata_len;
            }
            /* otherwise this digit length produces no terms and is skipped */
        }

        /* Extract the smallest k from the heap. */
        HeapNode cur = heap_pop(&heap);
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap << 1 : 4;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = cur.val;

        /* Advance the u pointer for this digit length. */
        DData *dd = &ddata[cur.d];
        dd->u_cur += 1;
        if (dd->u_cur <= dd->u_high) {
            seq_t val = dd->s * dd->u_cur * dd->u_cur;
            heap_push(&heap, (HeapNode){ val, (int)cur.d });
        }
    }

    return cache[n - 1];
}
