/*
 * A018336  Divisors of 210.
 * https://oeis.org/A018336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018336: Divisors of 210 in ascending order */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static seq_t divisors[16];
    static bool initialized = false;
    static size_t count = 0;

    if (!initialized) {
        const idx_t primes[4] = {2, 3, 5, 7};
        for (idx_t mask = 0; mask < (idx_t)1U << 4; ++mask) {
            seq_t prod = 1;
            for (int i = 0; i < 4; ++i) {
                if (mask & ((idx_t)1U << i))
                    prod *= (seq_t)primes[i];
            }
            divisors[count++] = prod;
        }
        qsort(divisors, count, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n == 0 || n > count)
        return 0;
    return divisors[n - 1];
}
