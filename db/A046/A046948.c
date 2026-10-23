/*
 * A046948  Sizes of successive balls in E_8 lattice.
 * https://oeis.org/A046948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046948
   Sizes of successive balls in the E8 lattice
   Formula: a(n) = 1 + 240 * Σ_{k=1}^{n} σ₃(k),  where σ₃(k) = Σ_{d|k} d³
   (with σ₃(0) defined as 0).  The series starts with a(0)=1.  This
   implementation caches intermediate cumulative sums to provide
   O(n) building time and O(1) retrieval time for any n. */

#include <stdlib.h>
#include <stdbool.h>

/* Sum of cubes of divisors of k. */
static seq_t sigma3(idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= k / d; ++d) {
        if (k % d == 0) {
            idx_t q = k / d;
            seq_t d3 = (seq_t)d * d * d;
            sum += d3;
            if (q != d) {
                seq_t q3 = (seq_t)q * q * q;
                sum += q3;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static cache of cumulative sums Σ_{k=1}^{i} σ₃(k) */
    static seq_t *S = NULL;     /* S[i] holds the sum up to i (S[0]=0) */
    static idx_t S_size = 0;    /* number of entries allocated (>=1) */
    static bool initialized = false;

    if (!initialized) {
        /* Allocate space for index 0 (sum = 0) */
        S = (seq_t *)malloc(sizeof(seq_t));
        if (!S) abort();      /* insufficient memory? */
        S[0] = (seq_t)0;
        S_size = 1;
        initialized = true;
    }

    /* Ensure the cache covers indices up to n */
    if ((idx_t)(n + 1) > S_size) {
        idx_t old_size = S_size;
        seq_t *tmp = (seq_t *)realloc(S, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        S = tmp;
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t s = sigma3(i);
            S[i] = S[i - 1] + s;
        }
        S_size = n + 1;
    }

    /* Compute a(n) using the cached cumulative sum */
    return (seq_t)1 + (seq_t)240 * S[n];
}
