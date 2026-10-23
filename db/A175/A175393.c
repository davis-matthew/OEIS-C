/*
 * A175393  a(n) = number of pairs (j,k), j <= k, of divisors of 2n such that
 *          j+k divides 2n.
 * https://oeis.org/A175393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(j,k) | j≤k, j|2n, k|2n, j+k|2n} (OEIS A175393)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Factorization helper for unsigned 64‑bit integers.  */
#include "../utils/factor_u64.h"

/* Compute all divisors of a 64‑bit number m and store them in the
   array `divs`.  `cnt` will contain the number of divisors.
   The function does not allocate any memory.  */
static void
enumerate_divisors(uint64_t m, uint64_t primes[], uint64_t exps[],
                   size_t pf_cnt, seq_t *divs, size_t *cnt)
{
    /* We build the divisor list iteratively.  First divisor is 1. */
    size_t cur_cnt = 1;
    divs[0] = (seq_t)1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t pow = 1;
        size_t added = 0;          /* Number of new divisors added at this level */

        for (uint64_t e = 1; e <= exps[i]; ++e) {
            pow *= p;              /* p^e  */

            for (size_t j = 0; j < cur_cnt; ++j) {
                divs[cur_cnt + added] = divs[j] * pow;
                ++added;
            }
        }
        cur_cnt += added;          /* Update current number of divisors */
    }
    *cnt = cur_cnt;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;    /* Out‑of‑range safety; offset starts at 1 */

    /* Compute 2n as a 64‑bit value.  For n beyond 2^63‑1 the result
       overflows, but the factorial of such a number is not required
       by the problem constraints.  */
    uint64_t M = (uint64_t)n * 2ULL;

    /* Factorise M.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt;
    factor_u64(M, primes, exps, &pf_cnt);

    /* Compute total number of divisors.  */
    size_t dcnt = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        dcnt *= (exps[i] + 1);

    /* Allocate array for all divisors.  */
    seq_t *divs = (seq_t *)malloc(dcnt * sizeof(seq_t));
    if (!divs)
        return (seq_t)0;   /* Minimal error handling */

    /* Enumerate all divisors.  */
    size_t div_cnt;
    enumerate_divisors(M, primes, exps, pf_cnt, divs, &div_cnt);

    /* Count qualified pairs (j ≤ k).  */
    seq_t count = 0;
    uint64_t M64 = (uint64_t)M;
    for (size_t i = 0; i < div_cnt; ++i) {
        for (size_t j = i; j < div_cnt; ++j) {
            seq_t sum = divs[i] + divs[j];
            uint64_t sum64 = (uint64_t)sum; /* safe: sum ≤ 2M ≤ 4n ≤ 8·10⁴ for n≤10⁴ */
            if (M64 % sum64 == 0)
                ++count;
        }
    }

    free(divs);
    return count;
}
