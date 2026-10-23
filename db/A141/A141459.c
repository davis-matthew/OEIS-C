/*
 * A141459  a(n) = Product_{p-1 divides n} p, where p is an odd prime.
 * https://oeis.org/A141459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Product_{p-1 | n, p odd prime} p (p ≥ 3)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 3779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates all divisors of n, tests each
 * corresponding p = d+1 for primality, and accumulates the product.
 * It works for any seq_t and idx_t defined by the evaluator. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute the product of all odd primes p such that p-1 divides n. */
static void dfs_div(uint64_t cur, int idx, int fac_cnt,
                    const uint32_t *pf, const int *exp,
                    seq_t *prod)
{
    if (idx == fac_cnt) {
        uint64_t p = cur + 1;          /* candidate prime */
        if (p >= 3) {
            bool isprime = true;
            for (uint64_t d = 2; d * d <= p; ++d) {
                if (p % d == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime) {
                /* Multiply the product by p. */
                *prod = (*prod) * (seq_t)p;
            }
        }
        return;
    }

    uint64_t mult = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        dfs_div(cur * mult, idx + 1, fac_cnt, pf, exp, prod);
        mult *= pf[idx];
    }
}

seq_t a(idx_t n)
{
    /* Special case: a(0) is defined to be 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Factor n into prime powers.  The numbers are small (≤ 10000),
     * so simple trial division is fast and sufficient. */
    uint64_t temp = (uint64_t)n;
    uint32_t pf[20];
    int exp[20];
    int fac_cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pf[fac_cnt] = (uint32_t)p;
            exp[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (temp > 1) {
        pf[fac_cnt] = (uint32_t)temp;
        exp[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* Generate all divisors and update the product. */
    seq_t prod = (seq_t)1;
    dfs_div((uint64_t)1, 0, fac_cnt, pf, exp, &prod);

    return prod;
}
