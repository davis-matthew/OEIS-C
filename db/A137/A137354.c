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
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   ipow_u64_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137354: lexicographically earliest subsequence of perfect powers
 * such that the first differences are an increasing sequence of primes. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility functions supplied by the build environment */
#include "../utils/ipow_u64_idx.h"
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
        heap = mem_resize(hpnode_t, heap, hcap);
    } else if (hsize == hcap) {
        hcap <<= 1;
        heap = mem_resize(hpnode_t, heap, hcap);
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
        hpnode_t n = { ipow_u64_idx(b, 2), b, 2 };
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
    seq = mem_resize(seq_t, seq, newcap);
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
        hpnode_t nxt = { ipow_u64_idx(cur.base, cur.exp + 1), cur.base, cur.exp + 1 };
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
