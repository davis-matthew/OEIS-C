/*
 * A003877  Degrees of irreducible representations of symmetric group S_13.
 * https://oeis.org/A003877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest (with multiplicity) of f^lambda over
 *            partitions lambda of 13, where f^lambda = number of standard
 *            Young tableaux, computed by the branching rule f^lambda =
 *            Sum_{corners c} f^(lambda - c), f^() = 1
 * Range:     u8 n <= 2, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   syt_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/syt_count.h"

static seq_t degrees_cache[101];
static int cache_size = 0;
static int cache_ready = 0;

static void gen_partitions(int *part, int plen, int remaining, int max_part) {
    if (remaining == 0) {
        if (cache_size < 101) {
            degrees_cache[cache_size++] = syt_count(part, plen);
        }
        return;
    }
    int limit = (remaining < max_part) ? remaining : max_part;
    for (int p = limit; p >= 1; p--) {
        part[plen] = p;
        gen_partitions(part, plen + 1, remaining - p, p);
    }
}

static void init_degrees(void) {
    if (cache_ready) return;
    int part[14];
    cache_size = 0;
    gen_partitions(part, 0, 13, 13);
    /* insertion sort ascending */
    for (int i = 1; i < cache_size; i++) {
        seq_t key = degrees_cache[i];
        int j = i - 1;
        while (j >= 0 && degrees_cache[j] > key) {
            degrees_cache[j + 1] = degrees_cache[j];
            j--;
        }
        degrees_cache[j + 1] = key;
    }
    cache_ready = 1;
}

seq_t a(idx_t n) {
    if (n < 1 || n > 101) return 0;
    init_degrees();
    if ((int)n > cache_size) return 0;
    return degrees_cache[n - 1];
}
