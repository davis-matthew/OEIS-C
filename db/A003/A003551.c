/*
 * A003551  Divisors of 2^46 - 1.
 * https://oeis.org/A003551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 2^46 - 1, in ascending order
 * Range:     u8 n <= 2, u16 n <= 2, u32 none, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   cmp_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_divisors.h"

#define MAX_DIVISORS 256

static seq_t divisors_cache[MAX_DIVISORS];
static int num_divisors_cached = -1;

static void compute_divisors(void) {
    if (num_divisors_cached >= 0) return;
    
    seq_t N = ((seq_t)1 << 46) - 1;
    int count = 0;
    
    for (seq_t d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            divisors_cache[count++] = d;
            if (d * d != N) {
                divisors_cache[count++] = N / d;
            }
        }
    }
    
    qsort(divisors_cache, count, sizeof(seq_t), cmp_divisors);
    num_divisors_cached = count;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    compute_divisors();
    
    if (n > (idx_t)num_divisors_cached) return 0;
    
    return divisors_cache[n - 1];
}
