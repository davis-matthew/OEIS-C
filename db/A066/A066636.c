/*
 * A066636  a(n) = A066638(n)/n, where A066638(n) is the smallest power of a
 *          squarefree kernel of n that is a multiple of n.
 * https://oeis.org/A066636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (A007947(n)^A051903(n))/n (Computed as the minimal
 *            multiplier m such that m*n has all prime‐power exponents equal.
 *            For each prime p|n, m includes p^(max_exp - exp_p) where max_exp
 *            is the maximum exponent among the prime factors of n.)
 * Range:     u8 n <= 111, u16 n <= 543, u32 n <= 5247, u64 n <= 9304,
 *            u128 n <= 9304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is the index; for n==0 the function is undefined,
       but callers start at 1. */
    if (n == 0) return (seq_t)0;          /* safety guard */

    uint64_t nn = (uint64_t)n;             /* use 64‑bit for factoring */
    if (nn == 1) return (seq_t)1;          /* radical(1)=1, exponent=0 */

    /* small tables for prime factors */
    int primes[16];
    int exps[16];
    int cnt = 0;

    uint64_t temp = nn;
    int max_exp = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = (int)p;
            exps[cnt]   = e;
            ++cnt;
            if (e > max_exp) max_exp = e;
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        primes[cnt] = (int)temp;
        exps[cnt]   = 1;
        ++cnt;
        if (1 > max_exp) max_exp = 1;
    }

    seq_t result = (seq_t)1;
    for (int i = 0; i < cnt; ++i) {
        int diff = max_exp - exps[i];
        for (int j = 0; j < diff; ++j)
            result *= (seq_t)primes[i];
    }

    return result;
}
