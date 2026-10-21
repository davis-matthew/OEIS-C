/*
 * A082018  First column of square array T(n,k) with T(1,1) = 1 where
 *          antidiagonals are filled alternating upwards and downwards with
 *          the smallest number not already used such that the n-th
 *          antidiagonal sum is a multiple of n.
 * https://oeis.org/A082018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first column of the square array T(n,k) where each
 *            antidiagonal is filled with the smallest unused integer so that
 *            the sum of the n‑th antidiagonal is a multiple of n. The array
 *            is filled from the top of an odd diagonal (downwards) to its
 *            bottom, and from the top of an even diagonal (upwar...
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* `seq_t` and `idx_t` are supplied by the build system.               */

static uint64_t   *used_bits  = NULL;   /* bitset of used integers          */
static size_t      used_cap   = 0;      /* allocated words                  */

/* Set bit for integer v (v >= 1). */
static void used_set(idx_t v)
{
    size_t word = v >> 6; /* divide by 64 */
    if (word >= used_cap) {
        size_t new_cap = word + 1;
        uint64_t *tmp = (uint64_t *)realloc(used_bits, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
        used_bits = tmp;
        used_cap  = new_cap;
    }
    used_bits[word] |= 1ULL << (v & 63);
}

/* Test whether integer v has been used. */
static bool used_test(idx_t v)
{
    size_t word = v >> 6;
    if (word >= used_cap) return false;
    return (used_bits[word] >> (v & 63)) & 1ULL;
}

/* ------------------------------------------------------------------- */
/* Simple binary min‑heap for skipped unused integers (C++'s
   priority_queue is not usable in C).                                    */
static idx_t   *heap      = NULL;
static size_t   heap_sz   = 0;
static size_t   heap_cap  = 0;

static void heap_push(idx_t x)
{
    if (heap_sz >= heap_cap) {
        size_t new_cap = (heap_cap == 0) ? 8 : heap_cap << 1;
        idx_t *tmp = (idx_t *)realloc(heap, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t pos = heap_sz++;
    heap[pos] = x;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent] <= heap[pos]) break;
        idx_t tmp = heap[parent];
        heap[parent] = heap[pos];
        heap[pos] = tmp;
        pos = parent;
    }
}

static idx_t heap_pop(void)
{
    if (heap_sz == 0) abort();          /* shouldn't happen */
    idx_t res = heap[0];
    heap[0] = heap[--heap_sz];
    size_t pos = 0;
    while (1) {
        size_t l = pos * 2 + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t sm = (r < heap_sz && heap[r] < heap[l]) ? r : l;
        if (heap[pos] <= heap[sm]) break;
        idx_t tmp = heap[pos];
        heap[pos] = heap[sm];
        heap[sm] = tmp;
        pos = sm;
    }
    return res;
}

/* ------------------------------------------------------------------- */
/* State for sequence generation                                         */
static seq_t *first_col = NULL;   /* 1‑based array of a(n) */
static size_t first_col_cap = 0;
static idx_t  max_computed = 0;
static idx_t  next_candidate = 2;  /* smallest number not yet examined */

/* Return the smallest unused integer and mark it used. */
static seq_t smallest_unused(void)
{
    if (heap_sz > 0) {
        idx_t val = heap_pop();
        used_set(val);
        return (seq_t)val;
    } else {
        while (used_test(next_candidate)) ++next_candidate;
        idx_t val = next_candidate;
        used_set(val);
        ++next_candidate;
        return (seq_t)val;
    }
}

/* Find the smallest unused integer >= start with residue rem (mod d). */
static seq_t find_last_residue(idx_t start, idx_t d, idx_t rem)
{
    idx_t v = start;
    while (1) {
        if (!used_test(v)) {
            if ((v % d) == rem) {
                used_set(v);
                if (v >= next_candidate) next_candidate = v + 1;
                return (seq_t)v;
            }
            /* keep it for later use */
            heap_push(v);
        }
        ++v;
    }
}

/* Build the array up to diagonal n (inclusive). */
static void compute_upto(idx_t n)
{
    if (n == 0) return;
    if (first_col == NULL || first_col_cap <= n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(first_col, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        first_col = tmp;
        first_col_cap = new_cap;
    }
    if (max_computed == 0) {
        /* a(1) = 1 */
        first_col[1] = (seq_t)1;
        used_set(1);
        next_candidate = 2;
        max_computed = 1;
    }
    for (idx_t d = max_computed + 1; d <= n; ++d) {
        idx_t sum_mod = 0;
        idx_t first_val = 0, prev_val = 0, last_val = 0;
        for (idx_t i = 1; i < d; ++i) {
            seq_t val_seq = smallest_unused();
            idx_t val = (idx_t)val_seq;
            if (i == 1) first_val = val;
            sum_mod = (sum_mod + val) % d;
            prev_val = val;
        }
        idx_t rem_needed = (d - sum_mod) % d;
        last_val = (idx_t)find_last_residue(prev_val + 1, d, rem_needed);
        first_col[d] = (d & 1) ? (seq_t)first_val : (seq_t)last_val;
    }
    max_computed = n;
}

/* Public interface required by the harness. */
seq_t a(idx_t n)
{
    compute_upto(n);
    return first_col[n];
}
