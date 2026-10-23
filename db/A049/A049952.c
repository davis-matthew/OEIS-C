/*
 * A049952  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A049952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = a(3) = 2 For n ≥ 4: let p be the unique integer
 *            with 2^p < n‑1 ≤ 2^(p+1) let m = 2*n – 3 – 2^(p+1) a(n) =
 *            a(1)+a(2)+…+a(n‑1) + a(m) (i.e. a(n) = S[n‑1] + a(m) where S[k]
 *            = Σ_{i=1}^k a(i))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    static seq_t *A = NULL;   /* a[ ] */
    static seq_t *S = NULL;   /* prefix sums: S[k] = a(1)+…+a(k) */
    static size_t cap = 0;    /* allocated size (>= n+1) */
    static idx_t last = 0;    /* highest index computed so far */

    if (n == 0) return (seq_t)0;   /* out of bounds – not used */

    if (n <= last) return A[n];

    /* grow arrays if needed (C++ requires explicit cast) */
    if (!cap || cap <= (size_t)n) {
        size_t newcap = cap ? cap << 1 : 64;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        S = (seq_t *)realloc(S, newcap * sizeof(seq_t));
        cap = newcap;
        if (last == 0) {          /* first allocation – initialise base */
            S[0] = (seq_t)0;
        }
    }

    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            A[i] = (seq_t)1;
            S[i] = A[i];
        } else if (i == 2 || i == 3) {
            A[i] = (seq_t)2;
            S[i] = S[i - 1] + A[i];
        } else {
            /* compute p = floor(log2(i-2))  (satisfies 2^p < i-1 ≤ 2^(p+1)) */
            idx_t p = floor_log2_uint64(i - 2);
            /* 2^(p+1) fits into 64‑bit for n ≤ 10000 */
            seq_t pow2 = (seq_t)((uint64_t)1 << (p + 1));
            idx_t m = (idx_t)(((seq_t)i * 2) - 3 - pow2);
            seq_t a_m = A[m];
            seq_t a_i = S[i - 1] + a_m;
            A[i] = a_i;
            S[i] = S[i - 1] + a_i;
        }
    }
    last = n;
    return A[n];
}
