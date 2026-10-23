/*
 * A008912  Truncated triangular numbers (of form n*(n-3)/2 - k^2+k*n+1 for
 *          1<=k<n).
 * https://oeis.org/A008912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers of the form (n^2+n+k^2-k+4kn)/2, n>=1, k>=0; The
 *            sequence is the sorted list of all distinct positive values of
 *            this expression taken for all n,k
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* helper to compute the expression for a given n and k */
static inline seq_t expr_val(idx_t n, idx_t k) {
    seq_t nn = (seq_t)n;
    seq_t kk = (seq_t)k;
    /* (nn*nn + nn + kk*kk - kk + 4*nn*kk) / 2 */
    return (nn * nn + nn + kk * kk - kk + (seq_t)4 * nn * kk) / 2;
}

/* vector to store the generated sequence */
static seq_t *seq_vals      = NULL;
static size_t seq_len        = 0;
static size_t seq_cap        = 0;

/* simple binary min‑heap for nodes (value, n, k) */
typedef struct {
    seq_t val;
    idx_t n;
    idx_t k;
} HeapNode;

static HeapNode *heap_arr      = NULL;
static size_t heap_len         = 0;
static size_t heap_cap         = 0;
static idx_t next_n_to_insert  = 1;       /* next n that hasn't been put into heap */
static seq_t last_output       = 0;        /* last value appended to seq_vals */

/* ensure capacity of seq_vals */
static void seq_ensure(size_t needed) {
    if (needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_vals = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ensure capacity of heap_arr */
static void heap_ensure(size_t needed) {
    if (needed > heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        heap_arr = (HeapNode*)realloc(heap_arr, new_cap * sizeof(HeapNode));
        heap_cap = new_cap;
    }
}

/* push a node onto the heap */
static void heap_push(HeapNode node) {
    heap_ensure(heap_len + 1);
    size_t i = heap_len++;
    heap_arr[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_arr[p].val <= heap_arr[i].val) break;
        HeapNode tmp = heap_arr[p];
        heap_arr[p] = heap_arr[i];
        heap_arr[i] = tmp;
        i = p;
    }
}

/* pop the minimal node from the heap */
static HeapNode heap_pop_min(void) {
    HeapNode res = heap_arr[0];
    heap_arr[0] = heap_arr[--heap_len];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap_len && heap_arr[left].val < heap_arr[smallest].val)
            smallest = left;
        if (right < heap_len && heap_arr[right].val < heap_arr[smallest].val)
            smallest = right;
        if (smallest == i) break;
        HeapNode tmp = heap_arr[i];
        heap_arr[i] = heap_arr[smallest];
        heap_arr[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/* main generator: ensure seq_vals contains at least target terms */
static void generate_upto(idx_t target) {
    while (seq_len < target) {
        /* Insert new n's as long as its first value can compete with current heap minimum */
        while (next_n_to_insert) {
            idx_t n = next_n_to_insert;
            seq_t val_n0 = expr_val(n, 0);
            if (heap_len == 0 || val_n0 <= heap_arr[0].val) {
                heap_push((HeapNode){val_n0, n, 0});
                next_n_to_insert++;
            } else {
                break;
            }
        }

        /* Take the smallest element from the heap */
        HeapNode cur = heap_pop_min();

        /* Record it if unique */
        if (seq_len == 0 || cur.val != last_output) {
            seq_ensure(seq_len + 1);
            seq_vals[seq_len++] = cur.val;
            last_output = cur.val;
        }

        /* Advance in the sequence corresponding to the same n */
        HeapNode nextNode = { expr_val(cur.n, cur.k + 1), cur.n, cur.k + 1 };
        heap_push(nextNode);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of bounds, though caller should only use n>=1 */
    generate_upto(n);
    return seq_vals[n - 1];
}
