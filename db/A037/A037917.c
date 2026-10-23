/*
 * A037917  Numbers n such that the Fibonacci number F(n) is divisible by a
 *          square.
 * https://oeis.org/A037917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The nth positive integer m such that Fibonacci(m) is divisible
 *            by a non‑trivial square
 * Range:     u8 none, u16 n <= 2, u32 n <= 328, u64 n <= 328, u128 n <= 328,
 *            big any n
 * Helpers:   rank_of_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The set of such m is the union of all arithmetic progressions {p·k·t : t ≥ 1}, where p is a prime and k is the smallest positive integer with p | F_k. */
/* The function a(idx_t n) returns the nth element of this set, indexed from 1. It uses a min‑heap merge of the progressions for all primes p ≤ 20000. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/rank_of_prime.h"

/* Helper structures for a binary min‑heap */
typedef struct {
    seq_t val;   /* current multiple */
    seq_t step;  /* fixed step = p * k */
} node_t;

typedef struct {
    node_t *data;      /* 1‑based array of nodes */
    size_t size;       /* number of elements in heap */
    size_t capacity;   /* allocated capacity */
} heap_t;

static void heap_init(heap_t *h, size_t cap) {
    h->capacity = cap;
    h->size = 0;
    h->data = (node_t*)malloc((cap + 1) * sizeof(node_t));
}

static void heap_free(heap_t *h) {
    free(h->data);
}

static void heap_swap(node_t *a, node_t *b) {
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(heap_t *h, node_t node) {
    size_t i = ++h->size;
    while (i > 1) {
        size_t parent = i >> 1;
        if (h->data[parent].val <= node.val) break;
        h->data[i] = h->data[parent];
        i = parent;
    }
    h->data[i] = node;
}

static node_t heap_pop(heap_t *h) {
    node_t min = h->data[1];
    node_t last = h->data[h->size--];
    size_t i = 1, child;
    while ((child = i << 1) <= h->size) {
        if (child + 1 <= h->size && h->data[child + 1].val < h->data[child].val)
            child++;
        if (last.val <= h->data[child].val) break;
        h->data[i] = h->data[child];
        i = child;
    }
    h->data[i] = last;
    return min;
}

/* Sieve of Eratosthenes up to MAX_PRIME. Returns array of primes and count by pointer. */
static void sieve_primes(uint32_t max_prime, uint32_t **primes_out, size_t *cnt_out) {
    uint8_t *is_composite = (uint8_t*)calloc(max_prime + 1, 1);
    size_t estimated = max_prime / 10;
    uint32_t *pr = (uint32_t*)malloc(estimated * sizeof(uint32_t));
    size_t cnt = 0;
    for (uint32_t p = 2; p <= max_prime; ++p) {
        if (!is_composite[p]) {
            if (cnt == estimated) {
                estimated <<= 1;
                pr = (uint32_t*)realloc(pr, estimated * sizeof(uint32_t));
            }
            pr[cnt++] = p;
            if ((uint64_t)p * p <= max_prime) {
                for (uint32_t q = p * p; q <= max_prime; q += p)
                    is_composite[q] = 1;
            }
        }
    }
    free(is_composite);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* Global arrays to hold primes and their progression steps (p * k). */
static uint32_t *global_primes = NULL;
static size_t global_prime_count = 0;
static seq_t *global_steps = NULL; /* step = p * k for each prime */

/* Initialization routine called once. */
static void ensure_initialized(void) {
    static bool init = false;
    if (init) return;
    /* Primes up to 20000 are sufficient for our n range. */
    uint32_t MAX_PRIME = 20000;
    sieve_primes(MAX_PRIME, &global_primes, &global_prime_count);
    global_steps = (seq_t*)malloc(global_prime_count * sizeof(seq_t));
    for (size_t i = 0; i < global_prime_count; ++i) {
        uint32_t p = global_primes[i];
        uint32_t k = rank_of_prime(p);
        global_steps[i] = ((seq_t)p) * ((seq_t)k);
    }
    init = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but avoid crash */
    ensure_initialized();

    /* Build a heap of all progressions. */
    heap_t h;
    heap_init(&h, global_prime_count);
    for (size_t i = 0; i < global_prime_count; ++i) {
        if (global_steps[i] == 0) continue;  /* should not happen */
        node_t node;
        node.val = global_steps[i];
        node.step = global_steps[i];
        heap_push(&h, node);
    }

    seq_t last_val = 0;
    size_t unique_count = 0;
    bool first = true;

    while (unique_count < (size_t)n) {
        node_t cur = heap_pop(&h);
        /* Skip duplicates */
        if (!first && cur.val == last_val) {
            /* do nothing */
        } else {
            ++unique_count;
            last_val = cur.val;
            if (unique_count == (size_t)n) {
                heap_free(&h);
                return cur.val;
            }
        }
        first = false;
        /* Push the next multiple for this progression */
        node_t next;
        next.val = cur.val + cur.step;
        next.step = cur.step;
        heap_push(&h, next);
    }

    heap_free(&h);
    return 0; /* unreachable */
}
