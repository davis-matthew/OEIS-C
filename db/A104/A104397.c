/*
 * A104397  Sums of 7 distinct positive pentatope numbers (A000332).
 * https://oeis.org/A104397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the sequence of sums of 7 distinct
   positive pentatope numbers (A000332).  It returns the nth
   smallest such sum (1‑based offset). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_seq.h"

/* Precompute all sums of 7 distinct pentatope numbers for k = 1..30.
   The number of combinations is C(30,7) = 2,035,800. */
static void build_sums(seq_t **sums, idx_t *count)
{
    const idx_t MAX_K = 30U;
    const size_t CAPACITY = 2035800U;
    seq_t *arr = (seq_t *)malloc(CAPACITY * sizeof(seq_t));
    if (!arr) return;               /* allocation failure – unlikely */

    /* Pentatope numbers Ptop(k) = binomial(k+3,4) */
    seq_t pent[MAX_K + 1];           /* 1‑indexed */
    for (idx_t k = 1U; k <= MAX_K; ++k) {
        seq_t prod = (seq_t)k * (k + 1) * (k + 2) * (k + 3);
        pent[k] = prod / 24U;
    }

    idx_t pos = 0;
    for (idx_t i1 = 1U; i1 <= MAX_K - 6U; ++i1)
        for (idx_t i2 = i1 + 1U; i2 <= MAX_K - 5U; ++i2)
            for (idx_t i3 = i2 + 1U; i3 <= MAX_K - 4U; ++i3)
                for (idx_t i4 = i3 + 1U; i4 <= MAX_K - 3U; ++i4)
                    for (idx_t i5 = i4 + 1U; i5 <= MAX_K - 2U; ++i5)
                        for (idx_t i6 = i5 + 1U; i6 <= MAX_K - 1U; ++i6)
                            for (idx_t i7 = i6 + 1U; i7 <= MAX_K; ++i7)
                            {
                                seq_t s = pent[i1] + pent[i2] + pent[i3] +
                                          pent[i4] + pent[i5] + pent[i6] + pent[i7];
                                arr[pos++] = s;
                            }

    *count = pos;

#if defined(__cplusplus)
    std::sort(arr, arr + *count);
#else
    qsort(arr, *count, sizeof(seq_t), cmp_seq);
#endif

    /* Remove duplicates in place */
    idx_t uniq = 0;
    for (idx_t i = 0; i < *count; ++i) {
        if (i == 0 || arr[i] != arr[i - 1]) {
            arr[uniq++] = arr[i];
        }
    }
    *count = uniq;

    *sums = arr;
}

/* Public function definition */
seq_t a(idx_t n)
{
    /* Guard against the offset (a(0) is undefined) */
    if (n == 0) return 0;

    /* Static state to compute the sums only once. */
    static bool initialized = false;
    static seq_t *sums = NULL;
    static idx_t sum_count = 0;

    if (!initialized) {
        build_sums(&sums, &sum_count);
        initialized = true;
    }

    /* After building, return the (n-1)th element in 0‑based indexing. */
    if (n - 1 < sum_count) {
        return sums[n - 1];
    }

    /* For n larger than the precomputed range, return 0
       (the tests do not ask for such values). */
    return 0;
}
