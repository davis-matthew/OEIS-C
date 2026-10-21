/*
 * A083896  Number of divisors of n with largest digit = 9 (base 10).
 * https://oeis.org/A083896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, has_digit9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of divisors of n whose decimal representation
   contains the digit 9 (base 10).  The sequence is OEIS A083896. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/has_digit9.h"

seq_t a(idx_t n)
{
    /* n is non‑zero index (offset 1).  Convert to uint64_t for arithmetic. */
    uint64_t N = (uint64_t)n;
    if (N <= 1) return (seq_t)0;   /* 1 has no divisor containing 9 */

    /* Factor n.  Allocate space for at least 20 prime factors (more than enough). */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pf_cnt = 0;
    factor_u64(N, primes, exps, &pf_cnt);

    /* Compute total number of divisors: product (exp+1) */
    size_t div_cnt = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        div_cnt *= (exps[i] + 1);

    /* Allocate array for divisors.  div_cnt <= 512 for N < 2^64, safe. */
    uint64_t *divs = (uint64_t *)malloc(div_cnt * sizeof(uint64_t));
    if (!divs) return (seq_t)0; /* allocation failure, return 0 */

    /* Generate all divisors iteratively. */
    size_t cur = 1;
    divs[0] = 1ULL;
    for (size_t i = 0; i < pf_cnt; ++i) {
        size_t start = cur;
        uint64_t p = primes[i];
        uint64_t pow = 1ULL;
        for (size_t k = 1; k <= exps[i]; ++k) {
            pow *= p;
            for (size_t j = 0; j < start; ++j)
                divs[cur++] = divs[j] * pow;
        }
    }

    /* Count divisors containing digit 9 */
    seq_t count = 0;
    for (size_t i = 0; i < cur; ++i)
        if (has_digit9(divs[i]))
            ++count;

    free(divs);
    return count;
}
