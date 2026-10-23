/*
 * A194741  Number of k such that {-k*sqrt(3)} > {-n*sqrt(3)}, where { } =
 *          fractional part.
 * https://oeis.org/A194741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k in [1..n] such that fractional part of
 *            k*sqrt(3) is strictly less than the fractional part of
 *            n*sqrt(3). The sequence is obtained by ranking each fractional
 *            part among the first n multiples of sqrt(3). This implementation
 *            computes the rank via a Fenwick tree (binary indexed tre...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   lower_bound_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/lower_bound_uint64.h"

static const uint64_t SCALE        = 1000000000000000000ULL;          /* 1e18 */
static const uint64_t SQRT3_SCALED = 1732050807568877294ULL;          /* round(sqrt(3)*1e18) */

/* comparison function for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t va = *(const uint64_t *)a;
    const uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Fenwick tree update: add 1 at position idx (1‑based) */
static void fenwick_add(size_t *bit, size_t n, size_t idx)
{
    while (idx <= n) {
        ++bit[idx];
        idx += idx & (~idx + 1);  /* idx += idx & -idx */
    }
}

/* Fenwick tree query: sum over [1..idx] (1‑based) */
static size_t fenwick_sum(const size_t *bit, size_t idx)
{
    size_t res = 0;
    while (idx > 0) {
        res += bit[idx];
        idx &= idx - 1;  /* idx -= idx & -idx */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence is defined for n ≥ 1 */

    size_t N = (size_t)n;
    /* allocate arrays */
    uint64_t *rem   = (uint64_t *)malloc(N * sizeof(uint64_t));
    uint64_t *sorted = (uint64_t *)malloc(N * sizeof(uint64_t));
    size_t *bit = (size_t *)calloc(N + 1, sizeof(size_t));

    if (!rem || !sorted || !bit) {
        /* allocation failed – return 0 as fallback */
        free(rem); free(sorted); free(bit);
        return 0;
    }

    /* compute remainders for 1..N */
    for (size_t i = 0; i < N; ++i) {
        unsigned __int128 prod = (unsigned __int128)(i + 1) * (unsigned __int128)SQRT3_SCALED;
        rem[i] = (uint64_t)(prod % SCALE);
    }

    /* copy and sort for compression */
    memcpy(sorted, rem, N * sizeof(uint64_t));
    qsort(sorted, N, sizeof(uint64_t), cmp_uint64);

    size_t result = 0;  /* to hold the final rank */

    for (size_t i = 0; i < N; ++i) {
        uint64_t value = rem[i];
        size_t rank = lower_bound_uint64(sorted, N, value) + 1;  /* 1‑based */

        if (i + 1 == n) {
            result = fenwick_sum(bit, rank - 1);  /* numbers < current */
            break;
        }
        fenwick_add(bit, N, rank);
    }

    free(rem); free(sorted); free(bit);
    return (seq_t)result;
}
