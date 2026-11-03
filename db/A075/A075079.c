/*
 * A075079  Numbers k in A001597 such that 2*k + 1 is prime.
 * https://oeis.org/A075079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   ipow_u64_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075079: numbers k in A001597 (perfect powers ≥1)
 * such that 2*k + 1 is prime.
 *
 * The function generates the sequence by repeatedly extracting the next
 * smallest perfect power from a min‑heap.  Each heap entry stores a
 * value, its exponent and the current base.  After an entry is
 * extracted, the next base for the same exponent is inserted.
 * The sequence values are stored in a dynamic array; duplicates are
 * discarded.  The result is memoised so subsequent calls are O(1).
 *
 * The implementation is independent of the concrete integer type
 * used for *seq_t* and *idx_t*; it only relies on the helper
 * function `is_prime_big(seq_t)` which works on the chosen big‑int
 * type.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_u64_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------------------------------------------------------------- */
/*  Node and heap structures                                         */
/* ----------------------------------------------------------------- */
typedef struct {
    seq_t val;        /* perfect‑power value */
    uint8_t exp;      /* exponent (≥2)        */
    uint64_t base;    /* current base          */
} Node;

typedef struct {
    Node *data;       /* array of nodes       */
    size_t size;      /* number of elements   */
} Heap;

/* ----------------------------------------------------------------- */
/*  Heap helper functions                                            */
/* ----------------------------------------------------------------- */
static void heap_init(Heap *h, size_t capacity)
{
    h->data = mem_new(Node, capacity);
    h->size = 0;
}

static void heap_free(Heap *h)
{
    mem_free(h->data);
}

static int heap_cmp(const Node *a, const Node *b)
{
    if (a->val < b->val) return -1;
    if (a->val > b->val) return 1;
    return 0;
}

static void heap_push(Heap *h, Node n)
{
    h->data[h->size++] = n;
    size_t i = h->size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_cmp(&h->data[p], &h->data[i]) <= 0) break;
        Node tmp = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = tmp;
        i = p;
    }
}

static Node heap_pop(Heap *h)
{
    Node res = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < h->size && heap_cmp(&h->data[l], &h->data[smallest]) < 0)
            smallest = l;
        if (r < h->size && heap_cmp(&h->data[r], &h->data[smallest]) < 0)
            smallest = r;
        if (smallest == i) break;
        Node tmp = h->data[i];
        h->data[i] = h->data[smallest];
        h->data[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/* ----------------------------------------------------------------- */
/*  Utility: compute base^exp  (seq_t)                              */

/* ----------------------------------------------------------------- */
/*  Initialise the heap with the first value for each exponent.      */
/* ----------------------------------------------------------------- */
static Heap *init_heap(void)
{
    /* 63 exponents are sufficient for the first 10 000 terms.   */
    size_t max_exp = 63;
    Heap *h = (Heap *)malloc(sizeof(Heap));
    heap_init(h, max_exp);
    for (int e = 2; e <= (int)max_exp; ++e) {
        Node nd = { ipow_u64_i32(2, e), (uint8_t)e, 2 };
        heap_push(h, nd);
    }
    return h;
}

/* ----------------------------------------------------------------- */
/*  Main sequence function                                           */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;    /* memoised terms */
    static size_t term_count = 0;  /* number stored */
    static seq_t last_val = 0;     /* last term stored for deduplication */
    static Heap *heap = NULL;      /* heap for perfect powers */
    static bool initialized = false;

    if (!initialized) {
        heap = init_heap();
        terms = mem_new(seq_t, 1);
        terms[0] = 1;                     /* a(1) = 1 */
        term_count = 1;
        last_val = 1;
        initialized = true;
    }

    /* Generate new terms until we have at least n of them */
    while (term_count < n) {
        Node nd = heap_pop(heap);

        if (nd.val != last_val) {              /* skip duplicates */
            seq_t cand = nd.val * ((seq_t)2) + 1;
            if (is_prime_big(cand)) {
                terms = mem_resize(seq_t, terms, (term_count + 1));
                terms[term_count++] = nd.val;
                last_val = nd.val;
            }
        }

        /* Insert next power with the same exponent */
        uint64_t next_base = nd.base + 1;
        Node next_node = { ipow_u64_i32(next_base, nd.exp),
                           nd.exp, next_base };
        heap_push(heap, next_node);
    }

    return terms[n - 1];
}
