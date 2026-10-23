/*
 * A169691  Let T be the sequence Fibonacci(2n+1), n>=0 (cf. A001519);
 *          sequence lists the differences T(j)-T(i) for i<j.
 * https://oeis.org/A169691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   cmp_seq_t, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169691:
   a(n) is the n-th smallest difference T(j)-T(i)
   where T(k)=Fibonacci(2k+1) and i<j.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */
#include "../utils/cmp_seq_t.h" /* provides int cmp_seq_t(const void*, const void*) */

/* Return number of pairs (i,j) with i<j for a given m */
static idx_t pair_count(idx_t m) {
    unsigned __int128 v = (unsigned __int128)m * (m - 1u) / 2u;
    return (idx_t)v;
}

/* Find the smallest m such that pair_count(m) >= n */
static idx_t find_m(idx_t n) {
    idx_t high = 2u;
    while (pair_count(high) < n) {
        if (high > UINT64_MAX / 2u) { high = UINT64_MAX; break; }
        high <<= 1u;
    }
    idx_t low = high >> 1u;
    while (low < high) {
        idx_t mid = low + (high - low) / 2u;
        if (pair_count(mid) >= n)
            high = mid;
        else
            low = mid + 1u;
    }
    return low;
}

/* Wrapper for qsort to compare seq_t values */
static int seq_cmp_wrapper(const void *a, const void *b) {
    return cmp_seq_t(a, b);
}

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)0; /* undefined for n<1, but return 0 */

    /* Step 1: find minimal m such that m*(m-1)/2 >= n */
    idx_t m = find_m(n);

    /* Step 2: compute Fibonacci values T[0..m-1] = Fib(2k+1) */
    seq_t *T = (seq_t *)malloc(m * sizeof(seq_t));
    if (!T) return (seq_t)0; /* allocation failure */
    for (idx_t k = 0; k < m; ++k) {
        T[k] = fib(2u * k + 1u);
    }

    /* Step 3: allocate array for all differences */
    idx_t diff_cnt = pair_count(m);
    seq_t *diffs = (seq_t *)malloc(diff_cnt * sizeof(seq_t));
    if (!diffs) {
        free(T);
        return (seq_t)0;
    }

    /* Step 4: compute differences */
    idx_t pos = 0;
    for (idx_t i = 0; i + 1 < m; ++i) {
        for (idx_t j = i + 1; j < m; ++j) {
            diffs[pos++] = T[j] - T[i];
        }
    }

    /* Step 5: sort differences */
    qsort(diffs, diff_cnt, sizeof(seq_t), seq_cmp_wrapper);

    seq_t result = diffs[n - 1u];

    /* Clean up */
    free(T);
    free(diffs);

    return result;
}
