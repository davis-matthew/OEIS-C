/*
 * A224326  Number of partitions of n into 3 distinct triangular numbers.
 * https://oeis.org/A224326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 distinct triangular
 *            numbers
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* allocate result array */
    seq_t *res = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!res) return 0; /* allocation failure, return 0 as safe fallback */

    for (idx_t i = 0; ; ++i) {
        seq_t Ti = (seq_t)i * (i + 1) / 2;
        if (Ti > n) break;           /* further i only increase Ti */

        for (idx_t j = i + 1; ; ++j) {
            seq_t Tj = (seq_t)j * (j + 1) / 2;
            if (Ti + Tj > n) break;   /* further j only increase Tj */

            for (idx_t k = j + 1; ; ++k) {
                seq_t Tk = (seq_t)k * (k + 1) / 2;
                seq_t sum = Ti + Tj + Tk;
                if (sum > n) break;    /* further k only increase Tk */
                res[(size_t)sum] += 1; /* count this partition */
            }
        }
    }

    seq_t result = res[(size_t)n];
    free(res);
    return result;
}
