/*
 * A003542  Divisors of 2^35 - 1.
 * https://oeis.org/A003542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 2^35 - 1 = 31 · 71 · 127 · 122921, sorted
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   div_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/div_cmp.h"

static seq_t div_cache[16];
static bool initialized = false;

static void init_divisors(void) {
    if (initialized) return;
    
    uint64_t primes[] = {31, 71, 127, 122921};
    int idx = 0;
    
    for (int b3 = 0; b3 <= 1; b3++) {
        for (int b2 = 0; b2 <= 1; b2++) {
            for (int b1 = 0; b1 <= 1; b1++) {
                for (int b0 = 0; b0 <= 1; b0++) {
                    seq_t d = 1;
                    if (b0) d *= (seq_t)primes[0];
                    if (b1) d *= (seq_t)primes[1];
                    if (b2) d *= (seq_t)primes[2];
                    if (b3) d *= (seq_t)primes[3];
                    div_cache[idx++] = d;
                }
            }
        }
    }
    
    qsort(div_cache, 16, sizeof(seq_t), div_cmp);
    initialized = true;
}

seq_t a(idx_t n) {
    init_divisors();
    if (n >= 1 && n <= 16) {
        return div_cache[n - 1];
    }
    return 0;
}
