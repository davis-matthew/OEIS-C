/*
 * A220848  a(n) = sum_(d|n) product_(d_x|n, d_x<=d) d_x.
 * https://oeis.org/A220848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n} ∏_{d_x|n, d_x≤d} d_x
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reference: https://oeis.org/A220848 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* comparison function for qsort on uint64_t */
static int cmp_u64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, guard */

    /* factor n into primes and exponents */
    uint64_t pr[20], ex[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* generate all divisors of n */
    uint64_t divs[256];
    size_t len = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t powp = 1;
        size_t cur_len = len;
        for (size_t e = 1; e <= ex[i]; ++e) {
            powp *= p;
            for (size_t j = 0; j < cur_len; ++j) {
                divs[len++] = divs[j] * powp;
            }
        }
    }

    /* sort divisors ascending */
    qsort(divs, len, sizeof(uint64_t), cmp_u64);

    /* compute prefix products and sum them */
    seq_t sum  = 0;
    seq_t prod = 1;
    for (size_t i = 0; i < len; ++i) {
        prod = prod * (seq_t)divs[i];
        sum  = sum  + prod;
    }

    return sum;
}
