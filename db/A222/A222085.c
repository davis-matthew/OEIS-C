/*
 * A222085  Sum of the least divisors of n whose LCM is equal to n.
 * https://oeis.org/A222085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the least divisors of n whose LCM is equal to n
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : ((x > y) ? 1 : 0);
}

/* Recursively generate all divisors of n from its prime factorisation. */
static void gen_divisors(size_t idx, uint64_t current,
                         const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, uint64_t *divs, size_t *div_cnt)
{
    if (idx == cnt) {
        divs[(*div_cnt)++] = current;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = current;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(idx + 1, val, pr, ex, cnt, divs, div_cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factorise n. */
    uint64_t primes[64], exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Generate all divisors. */
    uint64_t divisors[128];
    size_t div_cnt = 0;
    gen_divisors(0, 1ULL, primes, exps, prime_cnt, divisors, &div_cnt);

    /* Sort divisors ascending. */
    qsort(divisors, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Compute the required set of least divisors and their sum. */
    seq_t lcm = (seq_t)1;
    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divisors[i];
        sum += (seq_t)d;
        /* lcm = lcm(lcm, d) = lcm * (d / gcd(lcm, d)) */
        seq_t g = gcd(lcm, (seq_t)d);
        lcm = lcm * ((seq_t)d / g);
        if (lcm == (seq_t)n)
            break;   /* All necessary least divisors have been added */
    }

    return sum;
}
