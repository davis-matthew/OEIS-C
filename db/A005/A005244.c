/*
 * A005244  A self-generating sequence: start with 2 and 3, take all products
 *          of any 2 previous elements, subtract 1 and adjoin them to the
 *          sequence.
 * https://oeis.org/A005244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   generated in increasing order with a min-heap: a(1) = 2, a(2) =
 *            3, and the next term is the smallest a(i)*a(j)-1 (i<j) that is
 *            larger than the last term
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_zero, seq_is_fixed_width, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/zalloc_arr.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *A = NULL;
static idx_t A_len = 0, A_cap = 0;
static idx_t *H_i = NULL, *H_j = NULL;
static idx_t H_n = 0, H_cap = 0;
static bool fixed = false;
static seq_t maxv;

static bool h_less(idx_t x, idx_t y) {
    return A[H_i[x]] * A[H_j[x]] < A[H_i[y]] * A[H_j[y]];
}

static void h_swap(idx_t x, idx_t y) {
    idx_t t = H_i[x]; H_i[x] = H_i[y]; H_i[y] = t;
    t = H_j[x]; H_j[x] = H_j[y]; H_j[y] = t;
}

static void h_push(idx_t i, idx_t j) {
    if (fixed && A[j] > maxv / A[i]) return; /* product exceeds seq_t */
    if (H_n == H_cap) {
        H_cap = H_cap ? 2 * H_cap : 64;
        H_i = (idx_t *)realloc(H_i, H_cap * sizeof(idx_t));
        H_j = (idx_t *)realloc(H_j, H_cap * sizeof(idx_t));
    }
    idx_t k = H_n++;
    H_i[k] = i; H_j[k] = j;
    while (k > 0) {
        idx_t p = (k - 1) / 2;
        if (!h_less(k, p)) break;
        h_swap(k, p);
        k = p;
    }
}

static void h_pop(void) {
    H_n--;
    if (H_n == 0) return;
    H_i[0] = H_i[H_n]; H_j[0] = H_j[H_n];
    idx_t k = 0;
    for (;;) {
        idx_t l = 2 * k + 1, r = l + 1, m = k;
        if (l < H_n && h_less(l, m)) m = l;
        if (r < H_n && h_less(r, m)) m = r;
        if (m == k) break;
        h_swap(k, m);
        k = m;
    }
}

static void append(seq_t v) {
    if (A_len + 1 >= A_cap) {
        idx_t nc = A_cap * 2 + 16;
        A = grow_zero(A, A_cap, nc);
        A_cap = nc;
    }
    A[++A_len] = v;
}

seq_t a(idx_t n) {
    if (A == NULL) {
        fixed = seq_is_fixed_width();
        if (fixed) { seq_t z = 0; maxv = z - 1; }
        A_cap = 64;
        A = zalloc_arr(A_cap);
        A_len = 0;
        append(2);
        append(3);
        h_push(1, 2);
    }
    while (A_len < n) {
        if (H_n == 0) return 0; /* remaining terms do not fit in seq_t */
        idx_t i = H_i[0], j = H_j[0];
        seq_t v = A[i] * A[j] - 1;
        h_pop();
        if (i + 1 < j) h_push(i + 1, j);
        if (v > A[A_len]) {
            append(v);
            h_push(1, A_len);
        }
    }
    return A[n];
}
