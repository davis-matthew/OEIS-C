/*
 * A123364  Primes of the form a^2 + b^3 (with repetition).
 * https://oeis.org/A123364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form a² + b³ (with repetition),
 *            sorted ascending
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

#define BMAX 500

typedef struct {
    seq_t val;
    uint32_t b;
    uint32_t a;
} Node;

static Node heap[BMAX];
static size_t heap_cnt = 0;
static bool heap_inited = false;

static seq_t *primes = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;

static void heap_sift_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static void heap_sift_down(size_t i) {
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_cnt) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_cnt && heap[right].val < heap[left].val) smallest = right;
        if (heap[i].val <= heap[smallest].val) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void heap_push(uint32_t b, uint32_t a, seq_t val) {
    heap[heap_cnt].val = val;
    heap[heap_cnt].b   = b;
    heap[heap_cnt].a   = a;
    ++heap_cnt;
    heap_sift_up(heap_cnt - 1);
}

static Node heap_pop(void) {
    Node res = heap[0];
    heap[0] = heap[heap_cnt - 1];
    --heap_cnt;
    heap_sift_down(0);
    return res;
}

static void init(void) {
    if (heap_inited) return;
    heap_inited = true;
    for (uint32_t b = 1; b <= BMAX; ++b) {
        seq_t val = (seq_t)b * (seq_t)b * (seq_t)b + (seq_t)1; // a = 1
        heap_push(b, 1, val);
    }
}

static void ensure_primes_up_to(idx_t n) {
    init();
    while (prime_cnt < (size_t)n) {
        Node cur = heap_pop();
        if (is_prime_big(cur.val)) {
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 8;
                seq_t *new_arr = seq_realloc(primes, prime_cap, new_cap);
                if (!new_arr) return; /* Allocation failed – should not happen */
                primes = new_arr;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = cur.val;
        }
        uint32_t a_next = cur.a + 1;
        seq_t a_sq = (seq_t)a_next * (seq_t)a_next;
        seq_t b3   = (seq_t)cur.b * (seq_t)cur.b * (seq_t)cur.b;
        seq_t next_val = a_sq + b3;
        heap_push(cur.b, a_next, next_val);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_primes_up_to(n);
    return primes[n - 1];
}
