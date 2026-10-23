/*
 * A018738  Divisors of 948.
 * https://oeis.org/A018738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th (1-indexed) divisor of 948 in ascending order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the 12 divisors of 948. */
static seq_t divisors[12];
static size_t div_cnt = 0;
static bool initialized = false;

static void init_divisors(void) {
    if (initialized)
        return;
    initialized = true;

    /* 948 = 2^2 · 3^1 · 79^1 */
    const uint64_t primes[3] = { 2ULL, 3ULL, 79ULL };
    const uint8_t  exps[3]   = { 2U,   1U, 1U };

    size_t idx = 0;
    for (uint8_t e0 = 0; e0 <= exps[0]; ++e0) {
        seq_t p0 = 1;
        for (uint8_t i = 0; i < e0; ++i)
            p0 *= (seq_t)primes[0];

        for (uint8_t e1 = 0; e1 <= exps[1]; ++e1) {
            seq_t p1 = p0;
            for (uint8_t i = 0; i < e1; ++i)
                p1 *= (seq_t)primes[1];

            for (uint8_t e2 = 0; e2 <= exps[2]; ++e2) {
                seq_t p2 = p1;
                for (uint8_t i = 0; i < e2; ++i)
                    p2 *= (seq_t)primes[2];

                divisors[idx++] = p2;
            }
        }
    }
    div_cnt = idx;

    /* Sort the 12 divisors (insertion sort – trivial for 12 items). */
    for (size_t i = 1; i < div_cnt; ++i) {
        seq_t key   = divisors[i];
        size_t j    = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;

    init_divisors();

    if ((size_t)n > div_cnt)
        return (seq_t)0;

    return divisors[(size_t)n - 1];
}
