/*
 * A099228  Primes of the form m^k-k, with m and k > 1.
 * https://oeis.org/A099228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 921 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A099228: primes of the form m^k - k, with m,k > 1
   The function builds the sequence on demand, caching results
   for future calls.  The smallest prime is 2, followed by
   5, 7, 23, 47, 61, 79, 167, 223, 359, …                           */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>          /* UINT64_MAX */
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

/* Helper: compute base^exp in 64‑bit arithmetic with overflow detection.
   Returns true if the result fits in 64 bits, stored in *out.          */
static bool small_pow_uint64(uint64_t base, idx_t exp, uint64_t *out)
{
    __uint128_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
        if (result > UINT64_MAX) return false;
    }
    *out = (uint64_t)result;
    return true;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Heap node for min‑heap of candidates                       */
typedef struct {
    seq_t val;   /* candidate value m^k - k */
    idx_t k;     /* exponent */
    idx_t m;     /* base   */
} heap_node;

/* ------------------------------------------------------------------ */
/* dynamic min‑heap                                             */
static heap_node *heap        = NULL;
static size_t heap_size       = 0;
static size_t heap_cap        = 0;

static void heap_swap(heap_node *a, heap_node *b)
{
    heap_node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        heap_swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

static void heap_sift_down(size_t i)
{
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left  < heap_size && heap[left].val  < heap[smallest].val) smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val) smallest = right;
        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

static void heap_push(heap_node node)
{
    if (heap_size == heap_cap) {
        size_t new_cap = (heap_cap == 0) ? 64 : heap_cap << 1;
        heap_node *new_ptr = mem_resize(heap_node, heap, new_cap);
        if (!new_ptr) exit(1);
        heap = new_ptr;
        heap_cap = new_cap;
    }
    heap[heap_size++] = node;
    heap_sift_up(heap_size - 1);
}

static heap_node heap_pop(void)
{
    heap_node root = heap[0];
    heap[0] = heap[--heap_size];
    heap_sift_down(0);
    return root;
}

/* ------------------------------------------------------------------ */
/* cache for found primes                                          */
static seq_t *prime_cache = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;

static void ensure_prime_capacity(size_t n)
{
    if (prime_len >= n) return;
    size_t new_cap = (prime_cap == 0) ? 64 : prime_cap;
    while (new_cap < n) new_cap <<= 1;
    seq_t *new_ptr = mem_resize(seq_t, prime_cache, new_cap);
    if (!new_ptr) exit(1);
    prime_cache = new_ptr;
    prime_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* initialise the heap with the first candidate for each k>=2      */
static void init_heap(void)
{
    heap = NULL;
    heap_size = 0;
    heap_cap = 0;

    prime_cache = NULL;
    prime_len   = 0;
    prime_cap   = 0;

    /* push candidates for k from 2 until 2^k does not fit in 64 bits   */
    for (idx_t k = 2; ; ++k) {
        uint64_t p64;
        if (!small_pow_uint64(2, k, &p64)) break;    /* overflow, stop */
        heap_node node;
        node.val = (seq_t)p64 - k;
        node.k   = k;
        node.m   = 2;
        heap_push(node);
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                           */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_heap();
        initialized = true;
    }
    if (n == 0) return 0;   /* undefined for n=0 but harmless */

    /* grow cache until it holds at least n primes                   */
    if (prime_len < (size_t)n) {
        ensure_prime_capacity((size_t)n);
        while (prime_len < (size_t)n) {
            if (heap_size == 0) break;   /* safety guard */

            heap_node cur = heap_pop();

            /* skip duplicates in cache                            */
            if (prime_len == 0 || cur.val != prime_cache[prime_len-1]) {
                /* add to cache if prime                         */
                if (is_prime_big(cur.val)) {
                    prime_cache[prime_len++] = cur.val;
                }
            }

            /* generate next candidate for the same k              */
            idx_t m_next = cur.m + 1;
            if (m_next >= (idx_t)UINT64_MAX) continue;   /* too large base */
            uint64_t pow64;
            if (small_pow_uint64((uint64_t)m_next, cur.k, &pow64)) {
                heap_node next;
                next.val = (seq_t)pow64 - cur.k;
                next.k   = cur.k;
                next.m   = m_next;
                heap_push(next);
            }
            /* if pow overflows: that k produces no further values
               within 64‑bit range, so we simply stop adding that k   */
        }
    }

    return prime_cache[(size_t)n - 1];
}
