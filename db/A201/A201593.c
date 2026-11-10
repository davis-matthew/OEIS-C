/*
 * A201593  Largest value of y arising in A154777(n).
 * https://oeis.org/A201593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest y such that A154777(n) = x^2 + 2*y^2 with x,y >=
 *            1 (smallest x); computed by a heap generator for A154777 plus a
 *            two-pointer search
 * Range:     u8 n <= 81, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

typedef struct {
    idx_t x;
    idx_t y;
    seq_t val;
} A154777_node_t;

static seq_t *A154777_sequence = NULL;
static size_t A154777_seq_len  = 0;
static size_t A154777_seq_cap  = 0;

static A154777_node_t *A154777_heap = NULL;
static size_t A154777_heap_len = 0;
static size_t A154777_heap_cap = 0;

static idx_t A154777_next_x = 1;

static void A154777_swap_node(A154777_node_t *p, A154777_node_t *q)
{
    A154777_node_t tmp = *p;
    *p = *q;
    *q = tmp;
}

static void A154777_heap_grow(void)
{
    A154777_heap_cap = A154777_heap_cap ? A154777_heap_cap * 2U : 16U;
    A154777_node_t *tmp = mem_resize(A154777_node_t, A154777_heap, A154777_heap_cap);
    if (!tmp) abort();
    A154777_heap = tmp;
}

static void A154777_heap_push(idx_t x, idx_t y, seq_t val)
{
    if (A154777_heap_len == A154777_heap_cap) A154777_heap_grow();
    size_t i = A154777_heap_len++;
    A154777_heap[i].x   = x;
    A154777_heap[i].y   = y;
    A154777_heap[i].val = val;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (A154777_heap[p].val <= A154777_heap[i].val) break;
        A154777_swap_node(&A154777_heap[p], &A154777_heap[i]);
        i = p;
    }
}

static A154777_node_t A154777_heap_pop(void)
{
    A154777_node_t res = A154777_heap[0];
    A154777_heap[0] = A154777_heap[--A154777_heap_len];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < A154777_heap_len && A154777_heap[l].val < A154777_heap[smallest].val) smallest = l;
        if (r < A154777_heap_len && A154777_heap[r].val < A154777_heap[smallest].val) smallest = r;
        if (smallest == i) break;
        A154777_swap_node(&A154777_heap[i], &A154777_heap[smallest]);
        i = smallest;
    }
    return res;
}

static void A154777_ensure_n_terms(idx_t n)
{
    if (A154777_seq_len == 0) {
        A154777_next_x = 1;
        A154777_heap_len = 0;
    }

    while (A154777_seq_len < n) {
        seq_t cand = (seq_t)A154777_next_x * (seq_t)A154777_next_x + (seq_t)2U;
        if (A154777_heap_len == 0 || cand <= A154777_heap[0].val) {
            A154777_heap_push(A154777_next_x, 1U, cand);
            ++A154777_next_x;
            continue;
        }

        A154777_node_t cur = A154777_heap_pop();

        if (A154777_seq_len == 0 || cur.val != A154777_sequence[A154777_seq_len - 1]) {
            if (A154777_seq_len == A154777_seq_cap) {
                A154777_seq_cap = A154777_seq_cap ? A154777_seq_cap * 2U : 16U;
                seq_t *tmp = mem_resize(seq_t, A154777_sequence, A154777_seq_cap);
                if (!tmp) abort();
                A154777_sequence = tmp;
            }
            A154777_sequence[A154777_seq_len++] = cur.val;
        }

        idx_t y_next = cur.y + 1U;
        seq_t val_next = (seq_t)cur.x * (seq_t)cur.x
                         + (seq_t)2U * (seq_t)y_next * (seq_t)y_next;
        A154777_heap_push(cur.x, y_next, val_next);
    }
}

static seq_t A154777_a(idx_t n)
{
    if (n == 0U) return (seq_t)0;
    A154777_ensure_n_terms(n);
    return A154777_sequence[n - 1U];
}

seq_t a(idx_t n)
{
    seq_t m = A154777_a(n);
    seq_t x = 1;
    seq_t y = 1;
    /* largest y with x=1 possible: 2*y^2 < m */
    while ((seq_t)2 * (y + 1) * (y + 1) < m) ++y;
    while (y >= 1) {
        seq_t s = x * x + (seq_t)2 * y * y;
        if (s == m) return y;
        if (s < m) ++x;
        else --y;
    }
    return (seq_t)0;
}
