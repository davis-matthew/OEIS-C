/*
 * A071323  Alternating sum of all divisors of n; divisors nondecreasing,
 *          starting with 1.
 * https://oeis.org/A071323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=1}^{d} (-1)^(i-1) * d_i (d_i are the divisors of n
 *            in increasing order)
 * Range:     i8 n <= 130, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   _factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_factor_u64.h"

/* Comparison function for qsort on uint64_t values */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset starts at 1 */

    /* Factorise n */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    _factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Generate divisors */
    uint64_t divs[2048];
    size_t divcnt = 1;
    divs[0] = 1;                               /* divisor 1 */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];
        size_t current = divcnt;                /* number of divisors before extending */
        uint64_t pow = 1;
        for (unsigned k = 1; k <= e; ++k) {
            pow *= p;                          /* p^k */
            for (size_t j = 0; j < current; ++j)
                divs[divcnt++] = divs[j] * pow;
        }
    }

    /* Sort the divisor list */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* Alternating sum starting with +1 */
    seq_t sum = 0;
    bool add = true;
    for (size_t i = 0; i < divcnt; ++i) {
        if (add)
            sum += (seq_t)divs[i];
        else
            sum -= (seq_t)divs[i];
        add = !add;
    }

    return sum;
}
