/*
 * A003875  Degrees of irreducible representations of symmetric group S_11.
 * https://oeis.org/A003875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest (with multiplicity) of 11!/prod(hook
 *            lengths) over all partitions of 11 (hook length formula), sorted
 *            ascending
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: full, pass, 56 terms through n = 56 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t hook_product(const int *partition, int partition_len) {
    seq_t product = 1;
    for (int i = 0; i < partition_len; i++) {
        for (int j = 0; j < partition[i]; j++) {
            int hook = partition[i] - j;
            for (int k = i + 1; k < partition_len; k++) {
                if (partition[k] > j) {
                    hook++;
                }
            }
            product *= (seq_t)hook;
        }
    }
    return product;
}

static void generate_partitions(int *partition, int depth, int remaining, int max_part,
                                seq_t *cache, int *count) {
    if (remaining == 0) {
        seq_t fact11 = 1;
        for (int i = 1; i <= 11; i++) {
            fact11 *= (seq_t)i;
        }
        cache[*count] = fact11 / hook_product(partition, depth);
        (*count)++;
        return;
    }
    int limit = (max_part < remaining) ? max_part : remaining;
    for (int part = limit; part >= 1; part--) {
        partition[depth] = part;
        generate_partitions(partition, depth + 1, remaining - part, part, cache, count);
    }
}

seq_t a(idx_t n) {
    static seq_t cache[56];
    static int count = 0;
    static bool computed = false;

    if (!computed) {
        int partition[11];
        count = 0;
        generate_partitions(partition, 0, 11, 11, cache, &count);
        /* insertion sort ascending */
        for (int i = 1; i < count; i++) {
            seq_t key = cache[i];
            int j = i - 1;
            while (j >= 0 && cache[j] > key) {
                cache[j + 1] = cache[j];
                j--;
            }
            cache[j + 1] = key;
        }
        computed = true;
    }

    if (n < 1 || n > (idx_t)count) {
        return 0;
    }
    return cache[(int)n - 1];
}
