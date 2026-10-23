/*
 * A034089  Numbers that are proper divisors of the number you get by rotating
 *          digits right once.
 * https://oeis.org/A034089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 31, u128 n <= 74,
 *            big any n
 * Helpers:   dup_string, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034089: Numbers that are proper divisors of the number you get by
   rotating digits right once.  The algorithm generates all base periods
   `p(i/(10k-1))` for 2 ≤ k ≤ 9 and k ≤ i ≤ 9, ignores periods consisting
   of a single repeated digit, and uses a min‑heap of the current values of
   each base period.  For each node the string of the current value is
   extended by concatenating one more copy of the base period; the node
   is then sifted down the heap.  This generates the sequence in increasing
   numeric order.  The implementation stores each value as a decimal
   string, so it works for 8, 16, 32, 64 and arbitrary‑precision builds
   without overflow.  When a value is needed it is converted to `seq_t`
   with `seq_from_str`.  The function is memoised so that repeated calls
   are O(1).  */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"
#include "../utils/dup_string.h"

/* --------- Helper data structures ---------- */
typedef struct {
    size_t cur_len;    /* length of current decimal string */
    char *cur_str;     /* current decimal string, null terminated */
    size_t period_len; /* length of the base period string */
    char *period_str;  /* base period string, null terminated */
} Node;

/* --------- Heap operations (min‑heap) ---------- */
static Node *heap[40];          /* 1‑based heap, size ≤ 36 */
static size_t heap_sz = 0;

/* Comparator: smaller numeric value first */
static bool less_node(const Node *a, const Node *b) {
    if (a->cur_len != b->cur_len) return a->cur_len < b->cur_len;
    return strcmp(a->cur_str, b->cur_str) < 0;
}

static void heap_swap(size_t i, size_t j) {
    Node *tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heapify_up(size_t idx) {
    while (idx > 1 && less_node(heap[idx], heap[idx / 2])) {
        heap_swap(idx, idx / 2);
        idx /= 2;
    }
}

static void heapify_down(size_t idx) {
    while (true) {
        size_t l = idx * 2, r = l + 1, smallest = idx;
        if (l <= heap_sz && less_node(heap[l], heap[smallest])) smallest = l;
        if (r <= heap_sz && less_node(heap[r], heap[smallest])) smallest = r;
        if (smallest == idx) break;
        heap_swap(idx, smallest);
        idx = smallest;
    }
}

/* --------- Sequence storage ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* --------- Initialization of base periods ---------- */
static bool initialized = false;

static void init_sequence(void) {
    /* Generate all base periods */
    for (int k = 2; k <= 9; ++k) {
        int d = 10 * k - 1;
        for (int i = k; i <= 9; ++i) {
            char period_buf[50];
            int pos = 0;
            int rem = i;
            /* Compute repeating decimal of i/d */
            do {
                int prod = rem * 10;
                int digit = prod / d;
                period_buf[pos++] = '0' + digit;
                rem = prod % d;
            } while (rem != i);
            period_buf[pos] = '\0';
            int period_len = pos;

            /* Skip periods that are a single repeated digit */
            bool all_same = true;
            for (int j = 1; j < period_len; ++j)
                if (period_buf[j] != period_buf[0]) {
                    all_same = false;
                    break;
                }
            if (all_same) continue;

            /* Skip duplicate base periods */
            bool duplicate = false;
            for (size_t t = 1; t <= heap_sz; ++t) {
                if (heap[t]->period_len == period_len &&
                    strcmp(heap[t]->period_str, period_buf) == 0) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) continue;

            /* Allocate node */
            Node *node = (Node *)malloc(sizeof(Node));
            node->period_len = period_len;
            node->period_str = dup_string(period_buf, period_len);
            node->cur_len = period_len;
            node->cur_str = dup_string(period_buf, period_len);

            heap[++heap_sz] = node;
        }
    }

    /* Build min‑heap */
    for (size_t i = heap_sz / 2; i >= 1; --i) heapify_down(i);

    /* Allocate cache for sequence values */
    seq_cap = 1024;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    initialized = true;
}

/* --------- Public function ---------- */
seq_t a(idx_t n) {
    if (!initialized) init_sequence();
    if (n == 0) return (seq_t)0;          /* offset is 1; undefined for 0 */

    while (seq_len < (size_t)n) {
        /* Pop smallest node */
        Node *node = heap[1];
        seq_t val = seq_from_str(node->cur_str);
        if (seq_len >= seq_cap) {
            seq_cap <<= 1;
            seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        }
        seq_cache[seq_len++] = val;

        /* Extend node by concatenating one more period */
        size_t new_len = node->cur_len + node->period_len;
        char *new_str = (char *)malloc(new_len + 1);
        memcpy(new_str, node->cur_str, node->cur_len);
        memcpy(new_str + node->cur_len, node->period_str, node->period_len);
        new_str[new_len] = '\0';
        free(node->cur_str);
        node->cur_str = new_str;
        node->cur_len = new_len;

        /* Restore heap property */
        heapify_down(1);
    }
    return seq_cache[n - 1];
}
