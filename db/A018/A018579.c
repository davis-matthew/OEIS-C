/*
 * A018579  Divisors of 663.
 * https://oeis.org/A018579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 663 – finite ascending sequence */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static seq_t divisors[8];
static bool initialized = false;

static void compute_divisors(void)
{
    /* prime factors of 663 */
    const uint64_t primes[] = {3ULL, 13ULL, 17ULL};
    const size_t cnt = 3;

    size_t m = 0;
    for (uint64_t mask = 0; mask < (1ULL << cnt); ++mask) {
        uint64_t d = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i))
                d *= primes[i];
        }
        divisors[m++] = (seq_t)d;
    }

    /* sort ascending – trivial insertion sort for 8 elements */
    for (size_t i = 0; i < m - 1; ++i) {
        for (size_t j = i + 1; j < m; ++j) {
            if (divisors[i] > divisors[j]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined below offset */
    if (!initialized) compute_divisors();

    if (n <= 8) return divisors[n - 1];
    return (seq_t)0;               /* beyond the finite sequence */
}
