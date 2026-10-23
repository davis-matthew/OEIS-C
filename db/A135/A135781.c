/*
 * A135781  Positive numbers of the form x^3-3x y^2 (where x,y are positive
 *          integers).
 * https://oeis.org/A135781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135781: Positive numbers of the form x³ – 3·x·y² (x,y∈ℕ).
   This function generates all such values with x≥2, y≥1, keeps only
   the positive ones, sorts and removes duplicates, and returns the nth
   smallest distinct term (a(1)=2).  The implementation works for
   both C11 and C++17 builds by using an allocator that constructs objects
   in C++ mode and malloc/free in C mode. */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* Comparator used only when compiling as plain C.  In C++ we use std::sort
   with the operator< of seq_t. */
#ifndef __cplusplus
static int cmp_seq(const void *a, const void *b) {
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    return (*va < *vb) ? -1 : (*va > *vb) ? 1 : 0;
}
#endif

/* Helper allocator: constructs seq_t objects when compiled as C++.  In
   C mode the objects are trivially copyable, so malloc is sufficient. */
static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}
static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Compute the nth term.  The function is safe for any n≥1, independent of
   call order.  If n is larger than the number of distinct positive terms
   generated, 0 is returned. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* invalid index – behaviour below offset is undefined */

    /* Choose a bound that guarantees at least 10 000 distinct values.
       With x up to 400 we get ≈46 000 pairs, plenty for sorting. */
    const uint64_t XMAX = 400;

    /* Allocate space for the worst‑case number of generated values.
       The actual number may be smaller after deduplication. */
    size_t cap = 65536;               /* start with a reasonable capacity */
    seq_t *vals = alloc_seq_array(cap);
    if (!vals) return 0;              /* allocation failure */

    size_t len = 0;
    for (uint64_t x = 2; x <= XMAX; ++x) {
        /* y must satisfy 3·y² < x² to keep the value positive   */
        for (uint64_t y = 1;; ++y) {
            if ((uint64_t)3 * y * y >= x * x) break;

            /* Compute the value in complete seq_t arithmetic. */
            seq_t val = (seq_t)x * (seq_t)x * (seq_t)x
                        - (seq_t)3 * (seq_t)x * (seq_t)y * (seq_t)y;

            if (len == cap) {          /* grow buffer if needed */
                cap <<= 1;
                seq_t *tmp = alloc_seq_array(cap);
                if (!tmp) { free_seq_array(vals); return 0; }
                for (size_t i = 0; i < len; ++i) tmp[i] = vals[i];
                free_seq_array(vals);
                vals = tmp;
            }
            vals[len++] = val;
        }
    }

    /* Sort the array.  Use std::sort in C++ (operator< defined for seq_t),
       qsort in plain C. */
#ifdef __cplusplus
    std::sort(vals, vals + len);
#else
    qsort(vals, len, sizeof(seq_t), cmp_seq);
#endif

    /* Remove duplicates in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (i == 0 || vals[i] != vals[i - 1]) {
            vals[uniq++] = vals[i];
        }
    }

    /* Return the requested element, if it exists. */
    seq_t result = 0;
    if (n <= uniq) result = vals[n - 1];

    free_seq_array(vals);
    return result;
}
