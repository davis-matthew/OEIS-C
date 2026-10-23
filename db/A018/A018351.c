/*
 * A018351  Divisors of 242.
 * https://oeis.org/A018351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 242 in increasing order
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_divisors(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[6];
    const size_t divcnt = 6;

    if (!initialized) {
        /* 242 = 2 · 11² */
        const unsigned long long primes[2] = {2ULL, 11ULL};
        const int exps[2]          = {1, 2};

        size_t idx = 0;
        for (int e0 = 0; e0 <= exps[0]; ++e0) {
            seq_t v0 = 1;
            for (int i = 0; i < e0; ++i) v0 *= (seq_t)primes[0];
            for (int e1 = 0; e1 <= exps[1]; ++e1) {
                seq_t v1 = v0;
                for (int i = 0; i < e1; ++i) v1 *= (seq_t)primes[1];
                divisors[idx++] = v1;
            }
        }
        qsort(divisors, divcnt, sizeof(seq_t), cmp_divisors);
        initialized = true;
    }

    if (n > 0 && n <= divcnt)
        return divisors[n - 1];
    return (seq_t)0;   /* undefined for n > 6 */
}
