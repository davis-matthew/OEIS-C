/*
 * A087007  Divisors of 510510.
 * https://oeis.org/A087007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 122, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 510510 sorted increasingly (square‑free products of 2,3,5,7,11,13,17) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the 128 divisors */
static seq_t divisors[128];
static bool initialized = false;

/* compare function for qsort (uint64_t) */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n) {
    /* initialise once */
    if (!initialized) {
        static const uint64_t primes[7] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
        uint64_t tmp[128];
        uint64_t mask;

        for (mask = 0; mask < (1ULL << 7); ++mask) {
            uint64_t prod = 1ULL;
            int i;
            for (i = 0; i < 7; ++i)
                if (mask & (1ULL << i))
                    prod *= primes[i];
            tmp[mask] = prod;
        }
        qsort(tmp, 128, sizeof(uint64_t), cmp_uint64);
        int i;
        for (i = 0; i < 128; ++i)
            divisors[i] = (seq_t)tmp[i];
        initialized = true;
    }

    if (n == 0 || n > 128)
        return (seq_t)0;
    return divisors[n - 1];
}
