/*
 * A176703  Coefficients of a recursive polynomial based on Chaitin's S
 *          expressions: a(0)=1; a(1)=x; a(2)=1;
 *          a(n)=vector(a(n-1)).reverse(a(n-1)).
 * https://oeis.org/A176703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * a(n) is the n-th coefficient in the infinite sequence obtained
 *            by * concatenating the coefficient vectors of the polynomials
 *            b(k) * defined by * b(0) = 1 * b(1) = x * b(2) = 1 * b(n) =
 *            ∑_{i=1..n} b(i-1)·b(n-i) for n ≥ 3 * whose coefficient vector
 *            has length ⟨n⟩ = floor((n+1)/2)+1. * The sequenc...
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   poly_mul_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, calloc, free */
#include <stdbool.h>  /* for bool */
#include "../utils/poly_mul_add.h"

/* The types seq_t and idx_t are provided by the build environment. */
extern seq_t a(idx_t n);

#define MAX_K 512   /* safe for n up to 10000 */

/* Static tables for memoisation */
static seq_t *poly[MAX_K];          /* coefficient arrays for b(k)   */
static size_t   len_arr[MAX_K];     /* length of each poly[k]        */
static idx_t    cumulative[MAX_K];  /* start index of b(k) in a[·]  */
static idx_t    last_k   = 0;       /* highest computed k            */
static bool     init_done = false;

/* Helper: length of poly(n) = floor((n+1)/2)+1 */
static inline size_t poly_len(idx_t n)
{
    return ((size_t)(n) + 1) / 2 + 1;
}

/* Pre‑compute polynomials b(0)…b(k) until cumulative[k] > n. */
static void precompute(idx_t n)
{
    if (!init_done) {
        /* b(0) = 1 */
        poly[0] = (seq_t*)malloc(1 * sizeof(seq_t));
        poly[0][0] = (seq_t)1;
        len_arr[0] = 1;
        cumulative[0] = 1;

        /* b(1) = x   → coefficients {0,1} */
        poly[1] = (seq_t*)malloc(2 * sizeof(seq_t));
        poly[1][0] = (seq_t)0;
        poly[1][1] = (seq_t)1;
        len_arr[1] = 2;
        cumulative[1] = cumulative[0] + (idx_t)len_arr[1];

        /* b(2) = 1   → coefficients {1,0} (padded) */
        poly[2] = (seq_t*)malloc(2 * sizeof(seq_t));
        poly[2][0] = (seq_t)1;
        poly[2][1] = (seq_t)0;
        len_arr[2] = 2;
        cumulative[2] = cumulative[1] + (idx_t)len_arr[2];

        last_k = 2;
        init_done = true;
    }

    while (cumulative[last_k] <= n && last_k + 1 < MAX_K) {
        ++last_k;
        idx_t k = last_k;
        size_t len_k = poly_len(k);

        seq_t *res = (seq_t*)calloc(len_k, sizeof(seq_t));

        for (idx_t i = 1; i <= k; ++i) {
            const seq_t *p1 = poly[i - 1];
            const seq_t *p2 = poly[k - i];
            size_t len1 = len_arr[i - 1];
            size_t len2 = len_arr[k - i];
            poly_mul_add(p1, len1, p2, len2, res);
        }

        poly[k] = res;
        len_arr[k] = len_k;
        cumulative[k] = cumulative[k - 1] + (idx_t)len_k;
    }
}

seq_t a(idx_t n)
{
    precompute(n);

    /* Find the block k such that cumulative[k-1] ≤ n < cumulative[k] */
    idx_t k = 0;
    while (k <= last_k && n >= cumulative[k]) {
        ++k;
    }

    idx_t prev = (k == 0) ? 0 : cumulative[k - 1];
    size_t pos = (size_t)(n - prev);

    return poly[k][pos];
}
