/*
 * A047640  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^15 in powers of x.
 * https://oeis.org/A047640
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in (∏_{j≥1} (1-(-x)^j) - 1)^15.
 *            Computation via repeated convolution with the sparse base series
 * Range:     i8 n <= 17, i16 n <= 24, i32 n <= 251, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 9438 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Helper to build the sparse base series up to degree N.
   The base is ∏(1-(-x)^j) - 1, whose non‑zero coefficients are ±1
   at generalized pentagonal numbers. */
static void build_base(idx_t N, int **exp_ptr, int **sign_ptr, size_t *count_ptr) {
    int *exps = NULL;
    int *signs = NULL;
    size_t count = 0, cap = 16;

    exps = (int*)malloc(cap * sizeof(int));
    signs = (int*)malloc(cap * sizeof(int));
    if (!exps || !signs) exit(EXIT_FAILURE);

    for (int k = 1; ; ++k) {
        int64_t p1 = (int64_t)k * (3 * k - 1) / 2;
        if (p1 > (int64_t)N) break;
        if (count == cap) { cap *= 2; exps = (int*)realloc(exps, cap * sizeof(int)); signs = (int*)realloc(signs, cap * sizeof(int)); }
        exps[count] = (int)p1;
        signs[count] = ((p1 + k) & 1) ? -1 : 1;   /* sign for exponent p1   */
        ++count;

        int64_t p2 = (int64_t)k * (3 * k + 1) / 2;
        if (p2 > (int64_t)N) continue;
        if (count == cap) { cap *= 2; exps = (int*)realloc(exps, cap * sizeof(int)); signs = (int*)realloc(signs, cap * sizeof(int)); }
        exps[count] = (int)p2;
        signs[count] = ((p2 - k) & 1) ? -1 : 1;    /* sign for exponent p2   */
        ++count;
    }
    *exp_ptr   = exps;
    *sign_ptr  = signs;
    *count_ptr = count;
}

/* Global cache for computed values */
static seq_t *poly = NULL;       /* polynomial coefficients up to current max_n */
static size_t poly_size = 0;     /* poly allocated size (= current max_n + 1) */

/* Recompute the polynomial (base)^15 up to degree N and store in poly. */
static void recompute(idx_t N) {
    /* Build base series that is sparse and has coefficients ±1 */
    int *exp = NULL;
    int *sign = NULL;
    size_t base_count = 0;
    build_base(N, &exp, &sign, &base_count);

    /* Temporary arrays for convolution */
    seq_t *res_cur = mem_new(seq_t, N + 1);
    if (!res_cur) exit(EXIT_FAILURE);
    res_cur[0] = 1;                /* start with 1 */

    seq_t *res_next = mem_new(seq_t, N + 1);
    if (!res_next) exit(EXIT_FAILURE);

    /* Multiply 15 times by the sparse base */
    for (int step = 0; step < 15; ++step) {
        /* Zero the buffer for the next result */
        for (size_t i = 0; i <= (size_t)N; ++i) res_next[i] = 0;
        for (size_t i = 0; i <= (size_t)N; ++i) {
            seq_t val = res_cur[i];
            if (val == 0) continue;
            for (size_t t = 0; t < base_count; ++t) {
                size_t j = i + (size_t)exp[t];
                if (j > (size_t)N) break;  /* base exponents are ascending */
                if (sign[t] == 1)
                    res_next[j] += val;
                else
                    res_next[j] -= val;
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = res_cur;
        res_cur = res_next;
        res_next = tmp;
    }

    /* Free old polynomial if needed */
    if (poly) mem_free(poly);
    poly = res_cur;      /* res_cur now holds the final coefficients   */
    poly_size = (size_t)N + 1;

    /* Clean up base arrays */
    free(exp);
    free(sign);
    mem_free(res_next);      /* this buffer is unused after the last swap */
}

/* Public function requested by the task */
seq_t a(idx_t n) {
    if (n < 15) return 0;             /* below offset – unspecified, 0 is safe */
    if ((size_t)n >= poly_size) {
        /* enlarge cache to at least n */
        recompute(n);
    }
    return poly[n];
}
