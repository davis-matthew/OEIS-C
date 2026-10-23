/*
 * A179692  Numbers of the form p^9*q where p and q are distinct primes.
 * https://oeis.org/A179692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form p^9 * q where p and q are distinct primes */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

static bool initialized = false;
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static seq_t *pow9_vals = NULL;
static size_t p_cnt = 0;

struct Node {
    seq_t val;
    size_t p_idx;
    size_t q_idx;
};

static struct Node *heap = NULL;
static size_t heap_size = 0;

static seq_t *seq_vals = NULL;
static size_t seq_size = 0;
static size_t seq_cap = 0;

/* ---------- heap helpers ---------- */
static inline void heap_sift_up(size_t i) {
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (!(heap[i].val < heap[p].val)) break;
        struct Node tmp = heap[i];
        heap[i] = heap[p];
        heap[p] = tmp;
        i = p;
    }
}

static inline void heap_sift_down(size_t i) {
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && heap[l].val < heap[smallest].val)
            smallest = l;
        if (r < heap_size && heap[r].val < heap[smallest].val)
            smallest = r;
        if (smallest == i) break;
        struct Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void heap_push(struct Node node) {
    heap[heap_size++] = node;
    heap_sift_up(heap_size - 1);
}

static struct Node heap_pop(void) {
    struct Node top = heap[0];
    heap[0] = heap[--heap_size];
    heap_sift_down(0);
    return top;
}

/* ---------- initialization ---------- */
static void init(void) {
    if (initialized) return;
    initialized = true;

    /* generate a large enough list of primes for p and q */
    const idx_t LIMIT = 200000;   /* enough for the first 10 000 terms */
    size_t cnt;
    build_primes_upto(LIMIT, &prime_list, &cnt);
    prime_cnt = cnt;
    p_cnt = prime_cnt;          /* use all primes as possible p */

    /* allocate and compute p^9 for each prime */
    pow9_vals = (seq_t *)malloc(sizeof(seq_t) * p_cnt);
    for (size_t i = 0; i < p_cnt; ++i) {
        __uint128_t v = 1;
        for (int k = 0; k < 9; ++k)
            v *= prime_list[i];
        pow9_vals[i] = (seq_t)v;
    }

    /* allocate heap */
    heap = (struct Node *)malloc(sizeof(struct Node) * p_cnt);
    heap_size = 0;

    /* seed heap with the first q for each p (skipping q==p) */
    for (size_t ip = 0; ip < p_cnt; ++ip) {
        size_t iq = 0;
        while (iq < prime_cnt && prime_list[iq] == prime_list[ip]) ++iq;
        if (iq >= prime_cnt) continue;
        struct Node node;
        node.p_idx = ip;
        node.q_idx = iq;
        node.val   = pow9_vals[ip] * (seq_t)prime_list[iq];
        heap_push(node);
    }

    /* allocate initial cache for output sequence */
    seq_cap = 1024;
    seq_vals = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
    seq_size = 0;
}

/* generate sequence up to length n */
static void ensure(idx_t n) {
    if (n == 0) return;
    size_t need = (size_t)n;
    if (seq_size >= need) return;
    while (seq_size < need) {
        struct Node node = heap_pop();

        if (seq_size == seq_cap) {
            seq_cap <<= 1;
            seq_vals = (seq_t *)realloc(seq_vals, sizeof(seq_t) * seq_cap);
        }
        seq_vals[seq_size++] = node.val;

        size_t ip = node.p_idx;
        size_t iq = node.q_idx + 1;
        while (iq < prime_cnt && prime_list[iq] == prime_list[ip]) ++iq;
        if (iq < prime_cnt) {
            struct Node next;
            next.p_idx = ip;
            next.q_idx = iq;
            next.val = pow9_vals[ip] * (seq_t)prime_list[iq];
            heap_push(next);
        }
    }
}

seq_t a(idx_t n) {
    init();
    if (n == 0) return 0;
    ensure(n);
    return seq_vals[(size_t)n - 1];
}
