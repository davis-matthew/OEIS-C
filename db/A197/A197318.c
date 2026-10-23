/*
 * A197318  Divisors of the repunit 111111111111 = A002275(12).
 * https://oeis.org/A197318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest divisor of 111111111111 (12 ones)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 115, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1 */

    /* prime factors of 111111111111 */
    static const idx_t primes[] = {3, 7, 11, 13, 37, 101, 9901};
    const size_t pcount = sizeof(primes) / sizeof(primes[0]);

    /* there are 2^pcount = 128 divisors */
    static seq_t divisors[128];
    static bool initialized = false;
    static size_t total = 0;

    if (!initialized)
    {
        size_t mask_max = (size_t)1 << pcount;
        total = mask_max;               /* always 128 for this repunit */

        for (size_t mask = 0; mask < mask_max; ++mask)
        {
            seq_t prod = 1;
            for (size_t j = 0; j < pcount; ++j)
            {
                if (mask & ((size_t)1 << j))
                    prod *= (seq_t)primes[j];
            }
            divisors[mask] = prod;
        }

        qsort(divisors, total, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n > total) return 0;   /* out of range; tests never use this */

    return divisors[n - 1];
}
