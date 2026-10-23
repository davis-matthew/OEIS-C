/*
 * A178991  Primes that can be expressed as the sum of a Fibonacci number and
 *          the square of a Fibonacci number.
 * https://oeis.org/A178991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes that can be expressed as F(n) + F(m)^2, n,m >= 1. a(1) =
 *            2, a(2) = 3, a(3) = 5, a(4) = 7, ... Computation is done by
 *            generating all candidates of the form F(i) + F(j)^2 in
 *            increasing order using a min‑heap over the sequences indexed by
 *            j. Only those that are prime are kept in a static list...
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 29, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

/* Minimum Fibonacci index for the squared part */
static inline seq_t fib(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;
    if (len == 0) {            /* initialize cache with F(0)=0, F(1)=1 */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
    }
    while ((idx_t)len <= n) {
        if (len >= cap) {
            size_t new_cap = cap * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
        cache[len] = cache[len-1] + cache[len-2];
        ++len;
    }
    return cache[n];
}

/* ---------- heap for candidate values ---------- */
typedef struct {
    seq_t  val;
    idx_t  m;    /* index of the squared Fibonacci */
    idx_t  i;    /* index of the added Fibonacci   */
} Node;

static Node *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_sz  = 0;

static inline void heap_swap(size_t a, size_t b) {
    Node tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

static void heap_push(Node n) {
    if (heap_sz >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 64;
        heap = (Node *)realloc(heap, new_cap * sizeof(Node));
        heap_cap = new_cap;
    }
    heap[heap_sz++] = n;
    /* sift-up */
    size_t pos = heap_sz - 1;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[pos].val < heap[parent].val) {
            heap_swap(pos, parent);
            pos = parent;
        } else break;
    }
}

static Node heap_pop(void) {
    Node top = heap[0];
    heap_sz--;
    if (heap_sz > 0) {
        heap[0] = heap[heap_sz];
        /* sift-down */
        size_t pos = 0;
        while (1) {
            size_t l = pos * 2 + 1;
            size_t r = l + 1;
            size_t smallest = pos;
            if (l < heap_sz && heap[l].val < heap[smallest].val) smallest = l;
            if (r < heap_sz && heap[r].val < heap[smallest].val) smallest = r;
            if (smallest != pos) {
                heap_swap(pos, smallest);
                pos = smallest;
            } else break;
        }
    }
    return top;
}

/* ---------- storage of found primes ---------- */
static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_sz  = 0;

static void grow_primes(void) {
    if (primes_sz >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        primes = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        primes_cap = new_cap;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but guard */

    /* If we already have this value, return it. */
    if (n <= primes_sz) return primes[n-1];

    /* Continue generating until we have at least n terms. */
    /* next_m is the next Fibonacci index to start a new sequence. */
    static idx_t next_m = 1;  /* start with m = 1 */

    /* Add initial node if heap was empty. */
    while (heap_sz == 0) {
        seq_t fm = fib(next_m);
        seq_t val = fm * fm + fib(1);
        heap_push((Node){val, next_m, 1});
        next_m++;
    }

    while (primes_sz < (size_t)n) {
        /* Ensure all sequences whose first element is <= current
           minimum are present in the heap. */
        while (1) {
            if (next_m <= 0) break;   /* wrap around protection */
            seq_t fm = fib(next_m);
            seq_t val_first = fm * fm + fib(1);
            if (heap_sz == 0 || val_first <= heap[0].val) {
                heap_push((Node){val_first, next_m, 1});
                next_m++;
            } else break;
        }

        /* Pop the smallest candidate. */
        Node cur = heap_pop();
        /* If it's prime and not a duplicate, keep it. */
        if (is_prime_big(cur.val) &&
            (primes_sz == 0 || primes[primes_sz-1] != cur.val)) {
            grow_primes();
            primes[primes_sz++] = cur.val;
        }
        /* Push the next element from the same sequence. */
        idx_t next_i = cur.i + 1;
        seq_t fm = fib(cur.m);
        seq_t next_val = fm * fm + fib(next_i);
        heap_push((Node){next_val, cur.m, next_i});
    }

    return primes[n-1];
}
