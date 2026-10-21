/*
 * A078930  Let r+i*s be the sum of the distinct first-quadrant Gaussian
 *          integers dividing n; sequence gives r+s values.
 * https://oeis.org/A078930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of (Re(d)+Im(d)) over all Gaussian divisors d of n
 *            that lie in the first quadrant (Re(d) > 0, Im(d) >= 0)
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/factor_u64.h"          /* factor_u64 */
#include "../utils/_floor_isqrt_u64.h"    /* _floor_isqrt_u64 */

/* recursive generator for divisors of n^2 */
static void
gen_divisors(int idx, idx_t cur, const uint64_t *pr, const int *e, int cnt,
             idx_t *arr, size_t *size)
{
    if (idx == cnt) {
        arr[(*size)++] = cur;
        return;
    }
    for (int k = 0; k <= e[idx]; ++k) {
        gen_divisors(idx + 1, cur, pr, e, cnt, arr, size);
        cur *= pr[idx];
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* factor n */
    uint64_t primes[64];
    uint64_t exps_raw[64];
    size_t prime_cnt = 0;
    factor_u64(n, primes, exps_raw, &prime_cnt);

    /* build exponent array for n^2 */
    int exps[64];
    int total_divisors = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        exps[i] = (int)(exps_raw[i] * 2ULL);     /* exponent in n^2 */
        /* compute product of (exp+1) safely */
        total_divisors *= (exps[i] + 1);
    }

    /* generate all divisors of n^2 */
    idx_t *divs = (idx_t *)malloc(total_divisors * sizeof(idx_t));
    size_t divs_sz = 0;
    gen_divisors(0, (idx_t)1, primes, exps, (int)prime_cnt, divs, &divs_sz);

    seq_t sum = 0;

    for (size_t di = 0; di < divs_sz; ++di) {
        idx_t N = divs[di];                    /* N = r^2 + s^2 */
        idx_t rmax = _floor_isqrt_u64(N);      /* floor sqrt */

        for (idx_t r = 1; r <= rmax; ++r) {    /* r >= 1 as required */
            idx_t s2 = N - r * r;
            if (s2 == 0) {                     /* s = 0 case */
                /* s = 0, always satisfies square check */
            } else {
                idx_t s = _floor_isqrt_u64(s2);
                if (s * s != s2) continue;     /* not a perfect square */
                /* check divisibility conditions */
                if ((n * r) % N != 0) continue;
                if ((n * s) % N != 0) continue;
                sum += (seq_t)r + (seq_t)s;
                continue;
            }
            /* s = 0 case, check divisibility */
            if ((n * r) % N != 0) continue;
            sum += (seq_t)r;                    /* add r + 0 */
        }
    }

    free(divs);
    return sum;
}
