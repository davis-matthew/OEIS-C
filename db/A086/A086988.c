/*
 * A086988  Sum of odd indexed divisors of n.
 * https://oeis.org/A086988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd indexed divisors of n (i.e. a(n) = σ(n) –
 *            sum_of_even_indexed_divisors(n))
 * Range:     u8 n <= 195, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9572 (2026-11-01)
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes.h"

/* Recursive generation of all divisors from the prime factorisation. */
static void gen_divs(size_t idx,
                     uint64_t cur,
                     const uint64_t *p,
                     const int *e,
                     size_t cnt,
                     uint64_t *divs,
                     size_t *divcnt)
{
    if (idx == cnt) {
        divs[(*divcnt)++] = cur;
        return;
    }
    uint64_t val = 1;
    for (int i = 0; i <= e[idx]; ++i) {
        gen_divs(idx + 1, cur * val, p, e, cnt, divs, divcnt);
        val *= p[idx];
    }
}

static int cmp_u64(const void *a, const void *b)
{
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t num = (uint64_t)n;

    /* Lazily initialise a list of primes up to a fixed upper bound
       sufficient for all expected values of n (here 100000). */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static int primes_init = 0;
    if (!primes_init) {
        build_primes(100000, &primes, &primes_cnt);
        primes_init = 1;
    }

    /* Factorise n */
    uint64_t p[64];
    int e[64];
    size_t fac_cnt = 0;
    uint64_t temp = num;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            p[fac_cnt] = primes[i];
            int exp = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++exp;
            }
            e[fac_cnt] = exp;
            ++fac_cnt;
        }
    }
    if (temp > 1) {
        p[fac_cnt] = temp;
        e[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* Generate all divisors */
    uint64_t divisors[1024];
    size_t divcnt = 0;
    gen_divs(0, 1, p, e, fac_cnt, divisors, &divcnt);

    /* Sort the divisors */
    qsort(divisors, divcnt, sizeof(uint64_t), cmp_u64);

    /* Sum the divisors at odd positions (1‑based). */
    seq_t sum = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        if ((i & 1) == 0)  /* i = 0 => 1st divisor */
            sum += (seq_t)divisors[i];
    }

    return sum;
}
