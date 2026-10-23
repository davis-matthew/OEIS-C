/*
 * A018284  Divisors of 102.
 * https://oeis.org/A018284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 102 (all positive divisors of 102 in increasing
 *            order)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

static seq_t *init_divisors(void)
{
    static seq_t divisors[8];
    static bool inited = false;
    if (inited) return divisors;

    /* 102 = 2 * 3 * 17, each prime occurs once */
    const uint64_t primes[] = {2, 3, 17};
    const size_t pcount   = sizeof(primes) / sizeof(primes[0]);

    size_t idx = 0;
    for (uint64_t mask = 0; mask < (1ULL << pcount); ++mask) {
        seq_t prod = 1;
        for (size_t i = 0; i < pcount; ++i) {
            if (mask & (1ULL << i))
                prod *= (seq_t)primes[i];
        }
        divisors[idx++] = prod;
    }

    /* ensure ascending order */
    qsort(divisors, idx, sizeof(seq_t), cmp_seq);
    inited = true;
    return divisors;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t *divs = init_divisors();

    /* The divisor list has 8 terms.  For n > 8 we return 0. */
    if (n <= 8)
        return divs[n - 1];
    return 0;
}
