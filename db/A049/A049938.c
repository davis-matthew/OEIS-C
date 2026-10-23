/*
 * A049938  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A049938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{n-1} a(i) + a(n-1-2^p) * where p is the unique
 *            integer such that 2^p < n-1 ≤ 2^(p+1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Static cache for a(n) and its prefix sums. */
static seq_t *A = NULL;                /* a(n) values, 1‑based */
static seq_t *S = NULL;                /* prefix sums: S[i]=Σ_{j=1}^{i} a(j) */
static size_t capacity = 0;            /* allocated size (including index 0) */
static idx_t  computed = 0;            /* highest n that has been computed */

/* Helper: floor(log2(x)) for x > 0 */
static inline idx_t floor_log2_u64(idx_t x)
{
    /* x >= 1, x present as unsigned 64‑bit */
    return 63 - __builtin_clzll(x);
}

/* Grow cache to at least size n+1 (1‑based indexing). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= capacity)
        return;
    size_t new_cap = capacity ? capacity << 1 : 16;
    while (new_cap < (size_t)n + 1)
        new_cap <<= 1;
    A = (seq_t *)realloc((void *)A, (new_cap + 1) * sizeof(seq_t));
    S = (seq_t *)realloc((void *)S, (new_cap + 1) * sizeof(seq_t));
    assert(A && S);   /* abort if allocation failed */
    capacity = new_cap;
}

/* Compute a(n) using memoisation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but harmless */
    ensure_capacity(n);
    if (n <= computed)
        return A[n];

    /* Compute from computed+1 up to n. */
    for (idx_t k = computed + 1; k <= n; ++k)
    {
        if (k == 1) {
            A[1] = (seq_t)1;
            S[1] = (seq_t)1;
        }
        else if (k == 2) {
            A[2] = (seq_t)1;
            S[2] = S[1] + A[2];
        }
        else if (k == 3) {
            A[3] = (seq_t)2;
            S[3] = S[2] + A[3];
        }
        else {
            /* p = floor(log2(k-2)) */
            idx_t p = floor_log2_u64(k - 2);
            idx_t pow2 = 1ULL << p;
            idx_t m = k - 1 - pow2;          /* 1‑based index for a(m) */

            seq_t sum_prev = S[k - 1];         /* Σ_{i=1}^{k-1} a(i) */
            seq_t am = A[m];
            A[k] = sum_prev + am;              /* a(k) */
            S[k] = sum_prev + A[k];            /* prefix sum */
        }
    }
    computed = n;
    return A[n];
}
