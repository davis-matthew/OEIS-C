/*
 * A082245  Sum of (n-1)-th powers of divisors of n.
 * https://oeis.org/A082245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-1) (OEIS A082245)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   factor_uint64, pow_int_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_uint64.h"
#include "../utils/pow_int_seq.h"

/* Recursive helper that walks the divisor tree generated from the prime
 * factorization of n.  For each complete divisor, its (n-1)-th power is
 * added to *sum. */
static void dfs_div_sum(int idx, uint64_t cur,
                        const uint64_t *pr, const int *e, int cnt,
                        seq_t *sum, idx_t exp)
{
    if (idx == cnt) {
        *sum += pow_int_seq((seq_t)cur, exp);
        return;
    }

    uint64_t prod = cur;
    for (int k = 0; k <= e[idx]; ++k) {
        dfs_div_sum(idx + 1, prod, pr, e, cnt, sum, exp);
        prod *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined below offset */

    seq_t sum = 0;
    const idx_t exp = n - 1;

    uint64_t primes[20];
    int exps[20];
    int cnt;

    /* Factor n: primes[] holds the distinct prime factors, exps[] the powers. */
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    dfs_div_sum(0, 1ULL, primes, exps, cnt, &sum, exp);

    return sum;
}
