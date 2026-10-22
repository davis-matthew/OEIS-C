/*
 * A003876  Degrees of irreducible representations of symmetric group S_12.
 * https://oeis.org/A003876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest of f^lambda = 12!/prod(hook lengths), over
 *            all 77 partitions lambda of 12 (hook length formula), sorted in
 *            increasing order
 * Range:     u8 n <= 14, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   degree_from_partition, gen_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdint.h>
#include "../utils/degree_from_partition.h"
#include "../utils/gen_partitions.h"

seq_t a(idx_t n) {
    static uint64_t degrees[77];
    static int initialized = 0;

    if (!initialized) {
        int partitions[77][12];
        int current[12];
        int count = 0, i, j;
        gen_partitions(12, 12, current, 0, partitions, &count);
        for (i = 0; i < count; i++) degrees[i] = degree_from_partition(partitions[i]);
        for (i = 1; i < count; i++) {
            uint64_t v = degrees[i];
            j = i - 1;
            while (j >= 0 && degrees[j] > v) {
                degrees[j + 1] = degrees[j];
                j--;
            }
            degrees[j + 1] = v;
        }
        initialized = 1;
    }

    if (n >= 1 && n <= 77) {
        return (seq_t)degrees[(int)(n - 1)];
    }
    return (seq_t)0;
}
