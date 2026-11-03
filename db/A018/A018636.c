/*
 * A018636  Divisors of 770.
 * https://oeis.org/A018636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 770 in increasing order
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: full, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int compare_div(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

#define DIV_COUNT 16

seq_t a(idx_t n)
{
    /* number of divisors of 770 = 2^4 = 16 */
    static seq_t divisors[DIV_COUNT];
    static int initialized = 0;

    if (!initialized) {
        static const uint64_t primes[4] = {2, 5, 7, 11};
        for (idx_t mask = 0; mask < DIV_COUNT; ++mask) {
            seq_t prod = 1;
            for (idx_t i = 0; i < 4; ++i) {
                if (mask & (1ULL << i))
                    prod *= (seq_t)primes[i];
            }
            divisors[mask] = prod;
        }
        mem_qsort(divisors, DIV_COUNT, sizeof(seq_t), compare_div);
        initialized = 1;
    }

    if (n < 1 || n > DIV_COUNT)
        return 0; /* undefined for out-of-range indices */
    return divisors[n - 1];
}
