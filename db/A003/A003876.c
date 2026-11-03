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
 * Validated: full, pass, 77 terms through n = 77 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdint.h>

static uint64_t degree_from_partition(const int *partition) {
    uint64_t numerator = 1, denominator = 1;
    int i, j, k;
    for (i = 2; i <= 12; i++) numerator *= (uint64_t)i;
    for (i = 0; i < 12 && partition[i] != 0; i++) {
        for (j = 0; j < partition[i]; j++) {
            int hook = partition[i] - j;
            for (k = i + 1; k < 12 && partition[k] > j; k++) hook++;
            denominator *= (uint64_t)hook;
        }
    }
    return numerator / denominator;
}

static void gen_partitions(int n, int max_part, int *current, int pos, int partitions[][12], int *count) {
    if (n == 0) {
        int i;
        for (i = 0; i < 12; i++) partitions[*count][i] = (i < pos) ? current[i] : 0;
        (*count)++;
        return;
    }
    {
        int i;
        int start = n < max_part ? n : max_part;
        for (i = start; i >= 1; i--) {
            current[pos] = i;
            gen_partitions(n - i, i, current, pos + 1, partitions, count);
        }
    }
}

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
