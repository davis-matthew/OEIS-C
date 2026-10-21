/*
 * A061791  Number of distinct sums i^3 + j^3 for 1<=i<=j<=n.
 * https://oeis.org/A061791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums i^3 + j^3 with 1 ≤ i ≤ j ≤ n
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Number of pairs (i, j) with i <= j */
    size_t total = (size_t)n * ((size_t)n + 1) / 2;

    /* Allocate arrays for the sums and a temporary buffer for radix sort */
    uint64_t *arr = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!arr) return (seq_t)0;
    uint64_t *tmp = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!tmp) { free(arr); return (seq_t)0; }

    /* Generate all sums */
    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t icube = (uint64_t)i * i * i;
        for (idx_t j = i; j <= n; ++j) {
            uint64_t sum = icube + (uint64_t)j * j * j;
            arr[pos++] = sum;
        }
    }

    /* Radix sort (4 passes of 16 bits) */
    const uint32_t RADIX = 65536;
    uint32_t *count = (uint32_t *)malloc(RADIX * sizeof(uint32_t));
    if (!count) { free(arr); free(tmp); return (seq_t)0; }

    uint64_t *in  = arr;
    uint64_t *out = tmp;

    for (int shift = 0; shift < 64; shift += 16) {
        /* Histogram */
        for (uint32_t i = 0; i < RADIX; ++i) count[i] = 0;
        for (size_t k = 0; k < total; ++k) {
            uint32_t bucket = (uint32_t)((in[k] >> shift) & 0xFFFF);
            ++count[bucket];
        }

        /* Prefix sums to get starting indices */
        uint32_t acc = 0;
        for (uint32_t i = 0; i < RADIX; ++i) {
            uint32_t c = count[i];
            count[i] = acc;
            acc += c;
        }

        /* Reorder into out */
        for (size_t k = 0; k < total; ++k) {
            uint32_t bucket = (uint32_t)((in[k] >> shift) & 0xFFFF);
            out[count[bucket]++] = in[k];
        }

        /* Swap input and output buffers for next pass */
        uint64_t *tmp_ptr = in; in = out; out = tmp_ptr;
    }

    /* 'in' now points to the sorted array */
    uint64_t *sorted = in;

    /* Count distinct values in sorted array */
    seq_t distinct = 0;
    if (total > 0) {
        uint64_t prev = sorted[0];
        distinct = 1;
        for (size_t i = 1; i < total; ++i) {
            uint64_t cur = sorted[i];
            if (cur != prev) {
                ++distinct;
                prev = cur;
            }
        }
    }

    free(arr);
    free(tmp);
    free(count);

    return distinct;
}
