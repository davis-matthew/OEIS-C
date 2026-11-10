/*
 * A084388  a(n) = least positive k such that k^2 - A080761(n) is a cube.
 * https://oeis.org/A084388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k>0 with k^2 - A080761(n) a positive cube; found by
 *            a search upward from floor(sqrt(m))+1
 * Range:     u8 none, u16 none, u32 none, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   icbrt_seq, ipow_i32, isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/icbrt_seq.h"
#include "../utils/isqrt_seq.h"

static inline seq_t A080761_floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

#define A080761_X_MAX 5000

typedef struct {
    seq_t diff;
    idx_t x;
    seq_t y;
    seq_t inc;
} A080761_Node;

static A080761_Node *A080761_heap = NULL;
static size_t A080761_heap_cap = 0;
static size_t A080761_heap_size = 0;

static seq_t *A080761_cache = NULL;
static size_t A080761_cache_cap = 0;
static size_t A080761_cache_len = 0;
static seq_t A080761_prev_value = 0;

static inline void A080761_heap_swap(A080761_Node *a, A080761_Node *b)
{
    A080761_Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void A080761_heap_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (A080761_heap[parent].diff <= A080761_heap[i].diff)
            break;
        A080761_heap_swap(&A080761_heap[parent], &A080761_heap[i]);
        i = parent;
    }
}

static void A080761_heap_down(size_t i)
{
    while (true) {
        size_t left = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < A080761_heap_size && A080761_heap[left].diff < A080761_heap[smallest].diff)
            smallest = left;
        if (right < A080761_heap_size && A080761_heap[right].diff < A080761_heap[smallest].diff)
            smallest = right;
        if (smallest == i)
            break;
        A080761_heap_swap(&A080761_heap[i], &A080761_heap[smallest]);
        i = smallest;
    }
}

static void A080761_heap_push(const A080761_Node *val)
{
    if (A080761_heap_size == A080761_heap_cap) {
        size_t new_cap = (A080761_heap_cap == 0) ? 16 : A080761_heap_cap << 1;
        A080761_Node *tmp = mem_resize(A080761_Node, A080761_heap, new_cap);
        if (!tmp) abort();
        A080761_heap = tmp;
        A080761_heap_cap = new_cap;
    }
    A080761_heap[A080761_heap_size] = *val;
    A080761_heap_up(A080761_heap_size);
    A080761_heap_size++;
}

static A080761_Node A080761_heap_pop(void)
{
    A080761_Node top = A080761_heap[0];
    A080761_heap_size--;
    if (A080761_heap_size > 0) {
        A080761_heap[0] = A080761_heap[A080761_heap_size];
        A080761_heap_down(0);
    }
    return top;
}

static void A080761_ensure_cache(idx_t required_len)
{
    while (A080761_cache_len < required_len) {
        A080761_Node node = A080761_heap_pop();
        if (A080761_cache_len == 0 || node.diff != A080761_prev_value) {
            if (A080761_cache_len == A080761_cache_cap) {
                size_t new_cap = (A080761_cache_cap == 0) ? 16 : A080761_cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, A080761_cache, new_cap);
                if (!tmp) abort();
                A080761_cache = tmp;
                A080761_cache_cap = new_cap;
            }
            A080761_cache[A080761_cache_len++] = node.diff;
            A080761_prev_value = node.diff;
        }
        node.diff += node.inc;
        node.y += (seq_t)1;
        node.inc += (seq_t)2;
        A080761_heap_push(&node);
    }
}

static void A080761_init_heap(void)
{
    if (A080761_heap_size != 0)
        return;
    for (idx_t x = 1; x <= A080761_X_MAX; ++x) {
        seq_t xsq = (seq_t)x * (seq_t)x;
        seq_t x3 = xsq * (seq_t)x;
        seq_t t = A080761_floor_sqrt(x3);
        seq_t y = t + (seq_t)1;
        seq_t ysq = y * y;
        seq_t diff = ysq - x3;
        seq_t inc = y + y + (seq_t)1;
        A080761_Node node = { diff, x, y, inc };
        A080761_heap_push(&node);
    }
}

static seq_t A080761_a(idx_t n)
{
    if (n == 0)
        return 0;
    if (A080761_heap_size == 0)
        A080761_init_heap();
    A080761_ensure_cache(n);
    return A080761_cache[n - 1];
}

seq_t a(idx_t n)
{
    seq_t m = A080761_a(n);
    seq_t k = isqrt_seq(m) + 1;
    for (;; ++k) {
        seq_t d = k * k - m;
        seq_t c = icbrt_seq(d);
        if (c * c * c == d)
            return k;
    }
}
