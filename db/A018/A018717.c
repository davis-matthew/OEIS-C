/*
 * A018717  Divisors of 910.
 * https://oeis.org/A018717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 910 in ascending order
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparator for qsort. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    return (*va < *vb) ? -1 : (*va > *vb);
}

seq_t a(idx_t n)
{
    /* Prime factors of 910. */
    static const uint64_t primes[4] = {2, 5, 7, 13};
    static seq_t divs[16];
    static bool initialized = false;

    if (!initialized)
    {
        /* Generate all 16 divisors. */
        for (uint64_t mask = 0; mask < (1ULL << 4); ++mask)
        {
            seq_t val = 1;
            for (int i = 0; i < 4; ++i)
            {
                if (mask & (1ULL << i))
                    val *= (seq_t)primes[i];
            }
            divs[mask] = val;
        }
        qsort(divs, 16, sizeof(seq_t), cmp_seq);
        initialized = true;
    }

    if (n >= 1 && n <= (idx_t)16)
        return divs[(size_t)(n - 1)];
    return (seq_t)0;
}
