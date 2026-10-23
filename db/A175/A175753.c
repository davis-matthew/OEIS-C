/*
 * A175753  Numbers with 46 divisors.
 * https://oeis.org/A175753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest integer with 46 divisors: numbers of
 *            the form p^45 or p^22·q, where p,q are distinct primes
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Utility functions supplied by the build system */
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"   /* seq_mul(a,b) for arbitrary precision */
#include "../utils/big_pow.h"   /* big_pow(base,exp) for arbitrary precision */

typedef uint64_t idx_t;    /* index type (unsigned 64‑bit) */

/* ------------------------------------------------------------------------- */
/*  Helper: 64‑bit unsigned power (returns 0 if result would overflow).     */
static inline seq_t pow64(uint64_t base, int exp)
{
    __int128 t = 1;
    for (int i = 0; i < exp; ++i) {
        t *= base;
        if (t > (__int128)UINT64_MAX) return (seq_t)0;
    }
    return (seq_t)t;
}

/* ------------------------------------------------------------------------- */
/*  Min‑heap handling of candidates.  Only value is used for ordering.       */
typedef struct {
    seq_t val;          /* candidate value */
    idx_t p_idx;        /* index of the base prime p               */
    idx_t q_idx;        /* index of the multiplier prime q, or NO_Q */
} Node;

typedef struct {
    Node *data;
    size_t sz;
    size_t cap;
} Heap;

static const idx_t NO_Q = (idx_t)-1;

/* ---- heap routines ----------------------------------------------------- */
static inline void heap_init(Heap *h, size_t cap)
{
    h->cap = cap;
    h->sz  = 0;
    h->data = (Node *)calloc(cap, sizeof(Node));   /* 1‑based index usage */
}

static inline void heap_free(Heap *h)
{
    free(h->data);
}

static inline void swap(Node *a, Node *b)
{
    Node t = *a;  *a = *b;  *b = t;
}

static inline void heap_push(Heap *h, Node n)
{
    if (h->sz == h->cap) {                    /* grow if needed */
        h->cap = h->cap ? h->cap * 2 : 16;
        h->data = (Node *)realloc(h->data, h->cap * sizeof(Node));
    }
    h->data[++h->sz] = n;
    size_t i = h->sz;
    while (i > 1 && h->data[i/2].val > h->data[i].val) {
        swap(&h->data[i], &h->data[i/2]);
        i >>= 1;
    }
}

static inline Node heap_pop(Heap *h)
{
    Node top = h->data[1];
    h->data[1] = h->data[h->sz--];
    size_t i = 1;
    while (1) {
        size_t l = i << 1;
        if (l > h->sz) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r <= h->sz && h->data[r].val < h->data[l].val)
            smallest = r;
        if (h->data[smallest].val >= h->data[i].val) break;
        swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return top;
}

/* ------------------------------------------------------------------------- */
/*  Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------- */
    /*  Static state – primes are built once.                                 */
    /* --------------------------------------------------------------------- */
    static bool init = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!init) {
        /* A limit that provides many primes – enough for at least 10 000 terms. */
        const idx_t PRIME_LIMIT = 200000;              /* ≈ 17984 primes          */
        build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);
        init = true;
    }

    /* --------------------------------------------------------------------- */
    /*  Heap for generating the sequence in ascending order.                 */
    /* --------------------------------------------------------------------- */
    Heap heap;
    heap_init(&heap, prime_cnt * 2 + 5);     /* initial capacity */

    const bool small_bits = (sizeof(seq_t) <= 8);   /* 64‑bit unsigned?   */
    const uint64_t U64_MAX_VAL = (uint64_t)~0ULL;
    const __int128 U128_MAX_VAL = (__int128)U64_MAX_VAL;

    /* --------------------------------------------------------------------- */
    /*  Initialise heap with the smallest candidate from each prime.         */
    /* --------------------------------------------------------------------- */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];

        /* ---- p^45 ------------------------------------------------------- */
        if (small_bits) {
            /* Only 2^45 fits inside 64 bits. */
            if (p == 2) {
                seq_t val45 = pow64(p, 45);      /* guaranteed >0 */
                if (val45 != 0) {                 /* safety check */
                    heap_push(&heap, (Node){val45, i, NO_Q});
                }
            }
        } else {
            seq_t val45 = big_pow((seq_t)p, (seq_t)45);
            heap_push(&heap, (Node){val45, i, NO_Q});
        }

        /* ---- p^22 * q --------------------------------------------------- */
        size_t q_idx = 0;
        if (primes[0] == p) q_idx = 1;          /* skip p itself */

        if (q_idx < prime_cnt) {
            seq_t valq;
            if (small_bits) {
                seq_t p22 = pow64(p, 22);
                if (p22 != 0) {
                    __int128 prod = (__int128)p22 * primes[q_idx];
                    if (prod <= U128_MAX_VAL) {
                        valq = (seq_t)prod;
                    } else {
                        valq = (seq_t)0;   /* overflow – skip this candidate */
                    }
                } else {
                    valq = (seq_t)0;       /* overflow on p^22 – skip */
                }
            } else {
                seq_t p22 = big_pow((seq_t)p, (seq_t)22);
                valq = seq_mul(p22, (seq_t)primes[q_idx]);
            }
            if (valq != 0) {
                heap_push(&heap, (Node){valq, i, q_idx});
            }
        }
    }

    /* --------------------------------------------------------------------- */
    /*  Pull the smallest n values from the heap.                            */
    /* --------------------------------------------------------------------- */
    seq_t result = 0;
    for (idx_t cnt = 0; cnt < n; ++cnt) {
        Node cur = heap_pop(&heap);
        result = cur.val;

        /* If the node had a q (i.e., was of the form p^22*q), advance q.   */
        if (cur.q_idx != NO_Q) {
            idx_t next_q = cur.q_idx + 1;
            /* Skip the case where next_q == cur.p_idx (same prime).       */
            if (next_q == cur.p_idx) ++next_q;
            if (next_q < prime_cnt) {
                seq_t newval;
                if (small_bits) {
                    seq_t p22 = pow64(primes[cur.p_idx], 22);
                    if (p22 != 0) {
                        __int128 prod = (__int128)p22 * primes[next_q];
                        if (prod <= U128_MAX_VAL)
                            newval = (seq_t)prod;
                        else
                            newval = (seq_t)0;          /* overflow – skip */
                    } else {
                        newval = (seq_t)0;
                    }
                } else {
                    seq_t p22 = big_pow((seq_t)primes[cur.p_idx], (seq_t)22);
                    newval = seq_mul(p22, (seq_t)primes[next_q]);
                }
                if (newval != 0) {
                    heap_push(&heap, (Node){newval, cur.p_idx, next_q});
                }
            }
        }
    }

    heap_free(&heap);
    return result;
}
