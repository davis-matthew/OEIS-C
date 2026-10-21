/*
 * A218512  Number of partitions of n in which any two parts differ by at most
 *          10.
 * https://oeis.org/A218512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n in which any two parts differ
 *            by at most 10
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 205, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#include <cstdint>
#else
#include <stdint.h>
#endif

/* Helper macro to add two seq_t values safely when seq_t is 64‑bit */
#define ADD_INPLACE(a, b)                          \
    do {                                          \
        if (sizeof(seq_t) == 8) {                  \
            unsigned __int128 tmp =                \
                (unsigned __int128)(a) +            \
                (unsigned __int128)(b);             \
            a = (seq_t)tmp;                        \
        } else {                                   \
            a += (b);                               \
        }                                           \
    } while (0)

/* static storage for memoization */
static seq_t *A = NULL;
static size_t A_size = 0;

/* compute a[0..N] */
static void compute_upto(idx_t N) {
    size_t n = (size_t)N;
#if defined(__cplusplus)
    seq_t *newA = new seq_t[n + 1]();      /* value‑initialised to 0 */
    seq_t *dp   = new seq_t[n + 1]();
#else
    seq_t *newA = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *dp   = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif
    newA[0] = (seq_t)1;                     /* empty partition */

    for (size_t m = 1; m <= n; ++m) {
        size_t maxRem = n - m;              /* remaining sum after removing one m */
        /* zero dp[0..maxRem] */
        memset(dp, 0, (maxRem + 1) * sizeof(seq_t));
        dp[0] = (seq_t)1;

        size_t upper = m + 10;
        if (upper > n) upper = n;

        for (size_t s = m; s <= upper; ++s) {
            for (size_t t = s; t <= maxRem; ++t) {
                ADD_INPLACE(dp[t], dp[t - s]);
            }
        }

        for (size_t t = 0; t <= maxRem; ++t) {
            ADD_INPLACE(newA[m + t], dp[t]);
        }
    }

    /* free previous array */
    if (A != NULL) {
#if defined(__cplusplus)
        delete[] A;
#else
        free(A);
#endif
    }
    A      = newA;
    A_size = n + 1;
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n >= (idx_t)A_size)
        compute_upto(n);
    return A[(size_t)n];
}
