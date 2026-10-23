/*
 * A049928  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 4.
 * https://oeis.org/A049928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=4; * for n ≥ 4, * let p = ⌊log₂(n‑2)⌋ (so
 *            2ᵖ < n‑1 ≤ 2ᵖ⁺¹), * let m = 2·n‑3‑2ᵖ⁺¹, * then a(n) =
 *            (a(1)+…+a(n‑1)) – a(m)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes the values, so any order of calls works
 * and each call runs in amortised O(1) time after the first
 * extension of the cache. */

#include <stdlib.h>

/* floor(log₂(x)) for x ≥ 1, implemented with a simple loop so it
 * works on all compilers that support C11/C++17. */
static inline idx_t floor_log2_idx(idx_t x) {
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out‑of‑range, unused in OEIS */

    /* static memoization tables */
    static seq_t *A = NULL;   /* a[i] for i ≤ sz */
    static seq_t *C = NULL;   /* prefix sums: C[i] = a(1)+…+a(i) */
    static idx_t sz = 0;      /* current highest stored index */

    if (n > sz) {
        /* grow the tables to accommodate n */
        idx_t new_sz = n + 1;                  /* index 0 unused */
        A = (seq_t*)realloc(A, new_sz * sizeof(seq_t));
        C = (seq_t*)realloc(C, new_sz * sizeof(seq_t));

        /* compute new entries */
        for (idx_t i = sz + 1; i <= n; ++i) {
            if (i == 1) {
                A[i] = (seq_t)1;
                C[i] = A[i];
            } else if (i == 2) {
                A[i] = (seq_t)3;
                C[i] = C[i-1] + A[i];
            } else if (i == 3) {
                A[i] = (seq_t)4;
                C[i] = C[i-1] + A[i];
            } else {
                /* for n ≥ 4 */
                idx_t p = floor_log2_idx(i - 2);
                idx_t pow2 = ((idx_t)1) << (p + 1);   /* 2^(p+1) */
                idx_t m  = (idx_t)(2 * i - 3 - pow2);
                A[i] = C[i-1] - A[m];
                C[i] = C[i-1] + A[i];
            }
        }
        sz = n;
    }
    return A[n];
}
