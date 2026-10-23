/*
 * A182694  a(n) = the largest 3-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max {99 < k < 1000 : A000005(k)=n} if such k exists, else
 *            0
 * Range:     u8 n <= 1, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

#define MAX_DIV 32          /* the largest divisor count for a 3‑digit number */

static bool initialized = false;
static seq_t max_num[MAX_DIV + 1];

/* Pre‑compute the maximum 3‑digit number for each divisor count. */
static void init_cache(void) {
    for (idx_t i = 0; i <= MAX_DIV; ++i) {
        max_num[i] = (seq_t)0;
    }

    for (uint64_t k = 100; k <= 999; ++k) {
        idx_t div = (idx_t)divisor_count_u64(k);   /* divisor count of k */
        if (div <= MAX_DIV) {
            seq_t val = (seq_t)k;
            if (max_num[div] < val) {
                max_num[div] = val;
            }
        }
    }
    initialized = true;
}

/* Required function for the OEIS build interface. */
seq_t a(idx_t n) {
    if (!initialized) {
        init_cache();
    }

    if (n == 0 || n > MAX_DIV) {
        return (seq_t)0;
    }
    return max_num[n];
}
