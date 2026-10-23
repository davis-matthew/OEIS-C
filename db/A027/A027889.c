/*
 * A027889  Divisors of 999999999.
 * https://oeis.org/A027889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 999999999
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* comparison for qsort */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* cache of the 20 divisors */
    static bool initialized = false;
    static seq_t divisors[20];

    if (!initialized) {
        /* prime factorisation of 999999999 = 3^4 * 37 * 333667 */
        static const uint64_t primes[] = {3, 37, 333667};
        static const int exps[]   = {4, 1, 1};
        size_t idx = 0;

        /* generate all combinations of exponents */
        for (int e0 = 0; e0 <= exps[0]; ++e0) {
            seq_t v0 = 1;
            for (int i = 0; i < e0; ++i) v0 *= (seq_t)primes[0];
            for (int e1 = 0; e1 <= exps[1]; ++e1) {
                seq_t v1 = v0;
                for (int i = 0; i < e1; ++i) v1 *= (seq_t)primes[1];
                for (int e2 = 0; e2 <= exps[2]; ++e2) {
                    seq_t v = v1;
                    for (int i = 0; i < e2; ++i) v *= (seq_t)primes[2];
                    divisors[idx++] = v;
                }
            }
        }

        /* sort the divisors */
        qsort(divisors, 20, sizeof(seq_t), seq_cmp);
        initialized = true;
    }

    /* return the requested divisor, 0 if n is out of range */
    if (n >= 1 && n <= 20)
        return divisors[n - 1];
    return 0;
}
