/*
 * A232012  Numbers n such that p^2 + n is prime for some prime p.
 * https://oeis.org/A232012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232012: numbers n such that there exists a prime p for which p^2 + n is prime.
   The code generates all values n = q - p^2 where p and q are primes with q > p^2,
   collects them in ascending order, and removes duplicates. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

typedef struct {
    seq_t val;      /* candidate n = q - p^2   */
    size_t p_idx;   /* index of the prime p     */
    size_t q_idx;   /* index of the next prime q*/
} Node;

static bool initialized = false;
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static Node *heap = NULL;
static size_t heap_size = 0;
static seq_t *seq = NULL;
static size_t seq_size = 0;
static size_t seq_cap = 0;
static seq_t last_val = 0;
static bool last_set = false;

#define PRIME_LIMIT 20000000ULL   /* sufficient for the first 10 000 terms */

static void heap_sift_down(Node *h, size_t pos, size_t size) {
    while (1) {
        size_t left = 2 * pos + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < size && h[left].val < h[smallest].val) smallest = left;
        if (right < size && h[right].val < h[smallest].val) smallest = right;
        if (smallest == pos) break;
        Node tmp = h[pos];
        h[pos] = h[smallest];
        h[smallest] = tmp;
        pos = smallest;
    }
}

static void heap_sift_up(Node *h, size_t pos) {
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (h[parent].val <= h[pos].val) break;
        Node tmp = h[parent];
        h[parent] = h[pos];
        h[pos] = tmp;
        pos = parent;
    }
}

static void heap_push(Node *h, size_t *size, const Node *x) {
    h[*size] = *x;
    heap_sift_up(h, *size);
    (*size)++;
}

static Node heap_pop(Node *h, size_t *size) {
    Node ret = h[0];
    (*size)--;
    if (*size > 0) {
        h[0] = h[*size];
        heap_sift_down(h, 0, *size);
    }
    return ret;
}

static void init(void) {
    if (initialized) return;
    initialized = true;

    /* Build a list of primes up to PRIME_LIMIT. */
    build_primes(PRIME_LIMIT, &primes, &primes_cnt);

    /* Find the largest index i such that primes[i]^2 < PRIME_LIMIT. */
    size_t p_limit_idx = 0;
    while (p_limit_idx < primes_cnt &&
           (uint64_t)primes[p_limit_idx] * (uint64_t)primes[p_limit_idx] < PRIME_LIMIT) {
        ++p_limit_idx;
    }

    heap = (Node*)malloc(p_limit_idx * sizeof(Node));
    heap_size = 0;

    for (size_t i = 0; i < p_limit_idx; ++i) {
        idx_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        size_t q_idx = i + 1;
        while (q_idx < primes_cnt && primes[q_idx] <= p2) ++q_idx;
        if (q_idx < primes_cnt) {
            Node node;
            node.val = (seq_t)primes[q_idx] - (seq_t)p2;
            node.p_idx = i;
            node.q_idx = q_idx;
            heap_push(heap, &heap_size, &node);
        }
    }

    seq_cap = 128;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq_size = 0;
    last_set = false;
}

static void generate_until(idx_t need) {
    while (seq_size < need) {
        if (heap_size == 0) break;   /* no more candidates */

        Node cur = heap_pop(heap, &heap_size);

        /* Store if not a duplicate. */
        if (!last_set || cur.val != last_val) {
            if (seq_size == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_size++] = cur.val;
            last_val = cur.val;
            last_set = true;
        }

        /* Push the next q for the same p. */
        size_t pidx = cur.p_idx;
        size_t next_q = cur.q_idx + 1;
        if (next_q < primes_cnt) {
            idx_t p = primes[pidx];
            uint64_t p2 = (uint64_t)p * (uint64_t)p;
            Node node;
            node.val = (seq_t)primes[next_q] - (seq_t)p2;
            node.p_idx = pidx;
            node.q_idx = next_q;
            heap_push(heap, &heap_size, &node);
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1u) return 0;
    init();
    generate_until(n);
    return seq[n - 1];
}
