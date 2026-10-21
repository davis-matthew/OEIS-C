/*
 * A081530  a(n) = running sum of the first n harmonic numbers, multiplied by
 *          the LCM of 1..n.
 * https://oeis.org/A081530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1..n)*(n+1)*(H(n+1)-1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 18, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute lcm(1..n) by first finding all primes ≤ n */
    idx_t limit = n;

    /* sieve array */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!is_comp)
        return 0;

    /* dynamic array to store primes */
    size_t pr_cap = 64;
    uint64_t *primes = (uint64_t *)malloc(pr_cap * sizeof(uint64_t));
    if (!primes) {
        free(is_comp);
        return 0;
    }
    size_t pr_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (pr_cnt == pr_cap) {
                pr_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, pr_cap * sizeof(uint64_t));
                if (!tmp) {
                    free(primes);
                    free(is_comp);
                    return 0;
                }
                primes = tmp;
            }
            primes[pr_cnt++] = (uint64_t)i;

            if (i <= limit / i) {
                idx_t start = i * i;
                for (idx_t j = start; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);

    /* compute lcm by multiplying p^e for each prime p ≤ n,
       where e = floor(log_p(n)) */
    seq_t lcm = (seq_t)1;
    for (size_t pi = 0; pi < pr_cnt; ++pi) {
        uint64_t p = primes[pi];
        idx_t e = 0;
        idx_t pw = p;
        while (pw <= limit) {
            ++e;
            if (pw > limit / p)
                break;
            pw *= p;
        }
        seq_t p_pow = (seq_t)1;
        for (idx_t i = 0; i < e; ++i)
            p_pow *= (seq_t)p;
        lcm *= p_pow;
    }
    free(primes);

    /* compute the sum: Σ_{k=1}^n lcm/k * (n-k+1) */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t part = lcm / (seq_t)k;
        part *= (seq_t)(n - k + 1);
        sum += part;
    }

    return sum;
}
