/*
 * A139975  Primes of the form 20x^2+20xy+31y^2.
 * https://oeis.org/A139975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1861, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9470 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes of the form 20x² + 20xy + 31y², OEIS A139975.
   The function returns the nth prime (a(1) = 31).  The algorithm
   generates values in increasing order by using a min‑heap over all
   integer pairs (x, y) with y ≥ 1 and x starting at the vertex
   of the quadratic for that y.  Negative x are handled explicitly
   to avoid missing primes such as 239.  Values are tested for
   primality with the supplied is_prime_big function.  Duplicates
   are removed before adding to the prime list.  The routine is
   cache‑friendly and works for any build of seq_t. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* External utilities (for primality test) */
#include <stdint.h>
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

/* Node stored in the heap */
typedef struct {
    seq_t val;   /* value of 20x² + 20xy + 31y² */
    idx_t y;     /* fixed y */
    long long x; /* current x (may be negative) */
} node_t;

/*============================================================================*/
/*  Heap utilities (min‑heap on node_t::val)                                 */
/*============================================================================*/
static void heap_swap(node_t *a, node_t *b) {
    node_t t = *a; *a = *b; *b = t;
}

static void sift_down(node_t *h, idx_t size, idx_t i) {
    while (1) {
        idx_t left  = 2 * i + 1;
        idx_t right = left + 1;
        idx_t smallest = i;
        if (left < size && h[left].val < h[smallest].val) smallest = left;
        if (right < size && h[right].val < h[smallest].val) smallest = right;
        if (smallest == i) break;
        heap_swap(&h[i], &h[smallest]);
        i = smallest;
    }
}

static void sift_up(node_t *h, idx_t size, idx_t i) {
    while (i > 0) {
        idx_t parent = (i - 1) / 2;
        if (h[i].val < h[parent].val) {
            heap_swap(&h[i], &h[parent]);
            i = parent;
        } else break;
    }
}

static void heap_push(node_t *h, idx_t *size, idx_t capacity, node_t node) {
    /* capacity is guaranteed to be sufficient */
    h[*size] = node;
    sift_up(h, *size + 1, *size);
    (*size)++;
}

static node_t heap_pop(node_t *h, idx_t *size) {
    node_t top = h[0];
    h[0] = h[*size - 1];
    (*size)--;
    sift_down(h, *size, 0);
    return top;
}

/*============================================================================*/
/*  Compute value of the quadratic for given y and x (x may be negative)    */
/*============================================================================*/
static seq_t compute_value(idx_t y, long long x) {
    long long res = 20LL * x * x      /* 20x² */
                  + 20LL * x * (long long) y   /* 20xy */
                  + 31LL * (long long) y * (long long) y; /* 31y² */
    return (seq_t)res;
}

/*============================================================================*/
/*  Main sequence function                                                   */
/*============================================================================*/
seq_t a(idx_t n) {
    /* Static state for memoisation */
    static node_t *heap = NULL;
    static idx_t heap_size = 0;
    static idx_t HEAP_CAPACITY = 20000;   /* y range sufficient for n ≤ 10 000 */

    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;

    /* ------------------------------------------------------------------ */
    /*  Initialisation of heap and prime array on first call              */
    /* ------------------------------------------------------------------ */
    if (heap == NULL) {
        heap = mem_new(node_t, HEAP_CAPACITY);
        for (idx_t y = 1; y <= HEAP_CAPACITY; ++y) {
            /* start at the integer nearest to the vertex -y/2 */
            long long start = -((long long)(y + 1) / 2); /* floor(-(y+1)/2) */
            node_t node;
            node.y = y;
            node.x = start;
            node.val = compute_value(y, start);
            heap[heap_size++] = node;
        }
        /* heapify: build a min‑heap */
        for (idx_t i = heap_size / 2; i > 0; --i)
            sift_down(heap, heap_size, i - 1);
    }

    /* ------------------------------------------------------------------ */
    /*  Extend the prime list until it contains at least n primes         */
    /* ------------------------------------------------------------------ */
    while (primes_len < (size_t)n) {
        node_t cur = heap_pop(heap, &heap_size);

        /* Primality test */
        if (is_prime_big(cur.val)) {
            /* Avoid duplicates */
            if (primes_len == 0 || primes[primes_len - 1] != cur.val) {
                if (primes_len >= primes_cap) {
                    size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                    if (!tmp) return 0;          /* allocation failure */
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = cur.val;
            }
        }

        /* Advance in the same y‑sequence: increment x and push new value */
        cur.x++;
        cur.val = compute_value(cur.y, cur.x);
        heap_push(heap, &heap_size, HEAP_CAPACITY, cur);
    }

    return primes[n - 1];
}
