/*
 * A137354  Lexicographically earliest subsequence of the perfect powers in
 *          A001597 such that first differences are an increasing sequence of
 *          primes.
 * https://oeis.org/A137354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137354: lexicographically earliest subsequence of perfect powers
 * such that the first differences are an increasing sequence of primes. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility functions supplied by the build environment */
#include "../utils/pow_uint.h"      /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

/* --------------------------------------------------------------------- */
/* Min‑heap node for a perfect power                                         */
typedef struct {
    seq_t   val;        /* the power value */
    uint64_t base;      /* base of the power */
    unsigned exp;       /* current exponent (>=2) */
} hpnode_t;

/* Heap data ------------------------------------------------------------ */
static hpnode_t *heap   = NULL;
static size_t    hsize  = 0;
static size_t    hcap   = 0;
static bool      heap_inited = false;
static const uint64_t BASE_LIMIT = 200000;   /* enough for n up to 10000 */

/* Swap two heap elements ------------------------------------------------ */
static inline void hp_swap(hpnode_t *a, hpnode_t *b)
{
    hpnode_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a new node into the min‑heap ------------------------------------ */
static void hp_push(hpnode_t node)
{
    if (hcap == 0) {
        hcap = 64;
        heap = (hpnode_t *)realloc(heap, hcap * sizeof(hpnode_t));
    } else if (hsize == hcap) {
        hcap <<= 1;
        heap = (hpnode_t *)realloc(heap, hcap * sizeof(hpnode_t));
    }

    size_t i = hsize++;
    heap[i] = node;

    /* Bubble up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        hp_swap(&heap[p], &heap[i]);
        i = p;
    }
}

/* Pop the smallest node from the heap --------------------------------- */
static hpnode_t hp_pop(void)
{
    hpnode_t root = heap[0];
    heap[0] = heap[--hsize];

    /* Bubble down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= hsize) break;
        size_t r = l + 1;
        size_t s = l;
        if (r < hsize && heap[r].val < heap[l].val) s = r;
        if (heap[i].val <= heap[s].val) break;
        hp_swap(&heap[i], &heap[s]);
        i = s;
    }
    return root;
}

/* Initialise the heap with base² for all bases 2 … BASE_LIMIT --------- */
static void init_heap(void)
{
    if (heap_inited) return;
    for (uint64_t b = 2; b <= BASE_LIMIT; ++b) {
        hpnode_t n = { pow_uint(b, 2), b, 2 };
        hp_push(n);
    }
    heap_inited = true;
}

/* --------------------------------------------------------------------- */
/* Sequence storage ----------------------------------------------------- */
static seq_t *seq      = NULL;   /* memoised terms          */
static size_t seqlen   = 0;      /* number of stored terms  */
static size_t seccap   = 0;      /* allocated capacity      */
static seq_t  last_diff = 0;     /* difference of last two terms */
static seq_t  prev_val = 0;     /* last perfect power seen  */

/* Grow the memoisation array if needed -------------------------------- */
static void seq_grow(size_t needed)
{
    if (needed <= seccap) return;
    size_t newcap = seccap ? seccap << 1 : 64;
    while (newcap < needed) newcap <<= 1;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seccap = newcap;
}

/* --------------------------------------------------------------------- */
/* Main function defined by the exercise -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at offset 1 */

    init_heap();                   /* ensure heap is ready */
    seq_grow((size_t)n);           /* ensure memo array capacity */

    /* Initialise the very first term if we are computing from scratch */
    if (seqlen == 0) {
        seq[0] = 1;                /* the perfect power 1^2          */
        seqlen = 1;
        last_diff = 0;
        prev_val = 1;
    }

    /* Generate further terms until we have at least n of them */
    while (seqlen < (size_t)n) {
        hpnode_t cur = hp_pop();

        /* Push the next exponent for the same base */
        hpnode_t nxt = { pow_uint(cur.base, cur.exp + 1), cur.base, cur.exp + 1 };
        hp_push(nxt);

        /* Skip duplicate values (e.g. 64 from 8² and 4³) */
        if (cur.val == prev_val) continue;
        prev_val = cur.val;

        /* Compute the difference from the last stored term */
        seq_t diff = cur.val - seq[seqlen - 1];

        if (seqlen == 1) {          /* second term */
            if (diff > 0 && is_prime_big(diff)) {
                seq[seqlen++] = cur.val;
                last_diff = diff;
            }
        } else { /* third or later term */
            if (diff > last_diff && is_prime_big(diff)) {
                seq[seqlen++] = cur.val;
                last_diff = diff;
            }
        }
    }

    return seq[(size_t)n - 1];
}
