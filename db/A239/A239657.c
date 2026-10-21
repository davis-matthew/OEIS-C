/*
 * A239657  Number of odd divisors m of n such that there is a divisor d of n
 *          with d < m < 2*d.
 * https://oeis.org/A239657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors m of n such that there exists a
 *            divisor d of n with d < m < 2*d. * Method: factor n, enumerate
 *            all divisors, and for each odd divisor m check if there is a
 *            divisor d * satisfying d < m < 2*d. The count is accumulated in
 *            a seq_t variable
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static void gen_divisors_rec(const uint64_t *primes, const uint64_t *exps,
                             size_t idx, size_t pcnt,
                             uint64_t current, uint64_t *arr, size_t *pos)
{
    if (idx == pcnt) {
        arr[*pos] = current;
        (*pos)++;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t mult = 1;
    for (uint64_t k = 0; k <= exps[idx]; ++k) {
        gen_divisors_rec(primes, exps, idx + 1, pcnt,
                         current * mult, arr, pos);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n == 0, but return safe value */

    /* factor n */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(nn, primes, exps, &pcnt);

    /* compute total number of divisors (product of (exp+1)) */
    size_t div_cnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        div_cnt *= (size_t)(exps[i] + 1);

    /* allocate array for all divisors */
    uint64_t *divs = (uint64_t*)malloc(div_cnt * sizeof(uint64_t));
    if (!divs)
        return (seq_t)0;   /* allocation failure: return 0 as fallback */

    size_t pos = 0;
    gen_divisors_rec(primes, exps, 0, pcnt, (uint64_t)1, divs, &pos);

    seq_t count = (seq_t)0;

    /* for each odd divisor m, check if ∃ divisor d: d < m < 2*d */
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t m = divs[i];
        if ((m & 1) == 0)   /* skip even divisors */
            continue;

        bool found = false;
        for (size_t j = 0; j < div_cnt && !found; ++j) {
            if (i == j)
                continue;
            uint64_t d = divs[j];
            if (d < m && m < ((__uint128_t)d << 1))
                found = true;
        }
        if (found)
            count = count + (seq_t)1;
    }

    free(divs);
    return count;
}
