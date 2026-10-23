/*
 * A035689  Number of partitions of n into parts 8k+2 and 8k+7 with at least
 *          one part of each type.
 * https://oeis.org/A035689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k+2 and 8k+7 with
 *            at least one part of each type. Implementation: dynamic
 *            programming for partitions with both types and
 *            inclusion–exclusion (subtract partitions with only one type)
 * Range:     u8 n <= 72, u16 n <= 178, u32 n <= 511, u64 n <= 1652,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    /* size of arrays: need indices 0..n */
    size_t N = (size_t)(n) + 1;
#ifdef __cplusplus
    std::vector<seq_t> total(N, 0), onlyA(N, 0), onlyB(N, 0);
#else
    seq_t *total  = malloc(N * sizeof(seq_t));
    seq_t *onlyA  = malloc(N * sizeof(seq_t));
    seq_t *onlyB  = malloc(N * sizeof(seq_t));
    if (!total || !onlyA || !onlyB) {
        free(total); free(onlyA); free(onlyB);
        return 0;   /* allocation failed */
    }
    for (size_t i = 0; i < N; ++i) {
        total[i] = onlyA[i] = onlyB[i] = 0;
    }
#endif

    /* base case: one way to make sum 0 (empty partition) */
#ifdef __cplusplus
    total[0] = onlyA[0] = onlyB[0] = 1;
#else
    total[0] = onlyA[0] = onlyB[0] = 1;
#endif

    /* partitions using only parts of the form 8k+2 */
    for (size_t p = 2; p <= (size_t)n; p += 8) {
#ifdef __cplusplus
        for (size_t i = p; i < N; ++i)
            onlyA[i] += onlyA[i - p];
#else
        for (size_t i = p; i < N; ++i)
            onlyA[i] += onlyA[i - p];
#endif
    }

    /* partitions using only parts of the form 8k+7 */
    for (size_t p = 7; p <= (size_t)n; p += 8) {
#ifdef __cplusplus
        for (size_t i = p; i < N; ++i)
            onlyB[i] += onlyB[i - p];
#else
        for (size_t i = p; i < N; ++i)
            onlyB[i] += onlyB[i - p];
#endif
    }

    /* partitions using parts of both forms */
    for (size_t p = 2; p <= (size_t)n; p += 8) {
#ifdef __cplusplus
        for (size_t i = p; i < N; ++i)
            total[i] += total[i - p];
#else
        for (size_t i = p; i < N; ++i)
            total[i] += total[i - p];
#endif
    }
    for (size_t p = 7; p <= (size_t)n; p += 8) {
#ifdef __cplusplus
        for (size_t i = p; i < N; ++i)
            total[i] += total[i - p];
#else
        for (size_t i = p; i < N; ++i)
            total[i] += total[i - p];
#endif
    }

    /* inclusion–exclusion: at least one part of each type */
#ifdef __cplusplus
    seq_t ans = total[n];
    ans -= onlyA[n];
    ans -= onlyB[n];
#else
    seq_t ans = total[n];
    ans -= onlyA[n];
    ans -= onlyB[n];
    free(total); free(onlyA); free(onlyB);
#endif
    return ans;
}
