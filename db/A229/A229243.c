/*
 * A229243  Number A(n,k) of set partitions of {1,...,k*n} into sets of size
 *          at most n; square array A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A229243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence A229243, read anti–diagonally * from the
 *            two–dimensional array A(n,k) where * A(n,k) = (n*k)! * [x^(n*k)]
 *            exp(∑_{j=1..n} x^j / j!). * Equivalently, A(n,k) is the number
 *            of set partitions of * {1,…,k*n} into blocks of size at most n
 * Range:     u8 n <= 22, u16 n <= 30, u32 n <= 39, u64 n <= 69, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

/* ------------------------------------------------------------
 * Helper: compute B(m,n) = number of set partitions of a set
 * of size m into blocks of size at most n.
 * ------------------------------------------------------------ */
static seq_t compute_B(idx_t row, idx_t col) {
    /* If either dimension is zero, the set size is zero
       → exactly one partition. */
    if (row == 0 || col == 0) return (seq_t)1;

    /* total number of elements */
    idx_t m = row * col;

    /* dynamic programming array */
    seq_t *B = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!B) return (seq_t)0;   /* out of memory */

#ifdef __cplusplus
    /* construct each element (needed for arbitrary‑precision types) */
    for (size_t idx = 0; idx <= m; ++idx) new (B + idx) seq_t();
#endif

    B[0] = (seq_t)1;
    for (idx_t t = 1; t <= m; ++t) {
        seq_t sum = (seq_t)0;
        idx_t limit = (t < row) ? t : row;
        seq_t bin = (seq_t)1;            /* binom(t-1,0)         */
        for (idx_t i = 1; i <= limit; ++i) {
            sum += bin * B[t - i];
            if (i < limit) {
                /* binom(t-1,i) = binom(t-1,i-1) * (t-i) / i */
                bin = bin * (seq_t)(t - i) / (seq_t)i;
            }
        }
        B[t] = sum;
    }

    seq_t result = B[m];

#ifdef __cplusplus
    /* destruct elements */
    for (size_t idx = 0; idx <= m; ++idx) B[idx].~seq_t();
#endif
    free(B);
    return result;
}

/* ------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Locate the anti–diagonal containing index n.
       Find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    uint64_t s;
    {
        uint64_t low = 0;
        /* high is a rough upper bound for s */
        uint64_t high = (uint64_t)(sqrt((long double)n) * 2 + 10);
        while (low < high) {
            uint64_t mid = low + (high - low + 1) / 2;
            unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
            if (val <= n)
                low = mid;
            else
                high = mid - 1;
        }
        s = low;
    }

    /* coordinates on the array */
    uint64_t i = n - (s * (s + 1)) / 2;   /* row index  (original n) */
    uint64_t j = s - i;                   /* column index(original k) */

    return compute_B((idx_t)i, (idx_t)j);
}
