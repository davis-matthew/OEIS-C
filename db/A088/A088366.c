/*
 * A088366  a(n) = index of the n-th local maximum of A053707.
 * https://oeis.org/A088366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th i >= 2 with A053707(i) > A053707(i-1)
 *            and A053707(i) > A053707(i+1); found by a linear scan, with
 *            A053707(i) = A025475(i+1) - A025475(i)
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   build_primes, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/mem.h"

static inline seq_t A053707_mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

typedef struct {
    seq_t val;
    idx_t prime;
} A053707_Node;

static struct {
    A053707_Node *data;
    size_t size;
    size_t cap;
} A053707_heap = { NULL, 0, 0 };

static inline bool A053707_node_lt(const A053707_Node *a, const A053707_Node *b)
{
    return a->val < b->val;
}

static void A053707_heap_swap(size_t i, size_t j)
{
    A053707_Node tmp = A053707_heap.data[i];
    A053707_heap.data[i] = A053707_heap.data[j];
    A053707_heap.data[j] = tmp;
}

static void A053707_heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (!A053707_node_lt(&A053707_heap.data[i], &A053707_heap.data[parent]))
            break;
        A053707_heap_swap(i, parent);
        i = parent;
    }
}

static void A053707_heap_sift_down(size_t i)
{
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < A053707_heap.size && A053707_node_lt(&A053707_heap.data[left], &A053707_heap.data[smallest]))
            smallest = left;
        if (right < A053707_heap.size && A053707_node_lt(&A053707_heap.data[right], &A053707_heap.data[smallest]))
            smallest = right;

        if (smallest == i)
            break;
        A053707_heap_swap(i, smallest);
        i = smallest;
    }
}

static void A053707_heap_push(A053707_Node node)
{
    if (A053707_heap.size == A053707_heap.cap) {
        size_t new_cap = A053707_heap.cap ? (A053707_heap.cap << 1) : 16;
        A053707_Node *tmp = mem_resize(A053707_Node, A053707_heap.data, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        A053707_heap.data = tmp;
        A053707_heap.cap = new_cap;
    }
    A053707_heap.data[A053707_heap.size++] = node;
    A053707_heap_sift_up(A053707_heap.size - 1);
}

static A053707_Node A053707_heap_pop_min(void)
{
    A053707_Node min = A053707_heap.data[0];
    A053707_heap.data[0] = A053707_heap.data[--A053707_heap.size];
    A053707_heap_sift_down(0);
    return min;
}

static struct {
    seq_t *data;
    size_t len;
    size_t cap;
} A053707_powers = { NULL, 0, 0 };

static struct {
    idx_t *data;
    size_t len;
    size_t cap;
} A053707_primes = { NULL, 0, 0 };

static bool A053707_initialized = false;

static seq_t A053707_a(idx_t n)
{
    if (!A053707_initialized) {
        const idx_t PRIME_LIMIT = 200000;
        build_primes(PRIME_LIMIT, &A053707_primes.data, &A053707_primes.len);
        A053707_primes.cap = A053707_primes.len;

        A053707_powers.cap = 128;
        A053707_powers.len = 1;
        A053707_powers.data = mem_new(seq_t, A053707_powers.cap);
        if (!A053707_powers.data) exit(EXIT_FAILURE);
        A053707_powers.data[0] = (seq_t)1;

        for (size_t i = 0; i < A053707_primes.len; ++i) {
            idx_t p = A053707_primes.data[i];
            seq_t val = (seq_t)p * (seq_t)p;
            A053707_heap_push((A053707_Node){ val, p });
        }
        A053707_initialized = true;
    }

    while (A053707_powers.len <= (size_t)n) {
        A053707_Node cur = A053707_heap_pop_min();

        if (A053707_powers.len == A053707_powers.cap) {
            A053707_powers.cap <<= 1;
            if (A053707_powers.cap == 0) A053707_powers.cap = 16;
            seq_t *tmp = mem_resize(seq_t, A053707_powers.data, A053707_powers.cap);
            if (!tmp) exit(EXIT_FAILURE);
            A053707_powers.data = tmp;
        }
        A053707_powers.data[A053707_powers.len++] = cur.val;

        seq_t next_val = A053707_mul_small(cur.val, (unsigned)cur.prime);
        A053707_heap_push((A053707_Node){ next_val, cur.prime });
    }

    return A053707_powers.data[n] - A053707_powers.data[n - 1];
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i;
    seq_t prev = A053707_a(1);
    seq_t cur = A053707_a(2);
    for (i = 2; ; i++) {
        seq_t next = A053707_a(i + 1);
        if (cur > prev && cur > next) {
            count++;
            if (count == n) return (seq_t)i;
        }
        prev = cur;
        cur = next;
    }
}
