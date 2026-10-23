/*
 * A138652  Number of differences (not all necessarily distinct) between
 *          consecutive divisors of 2n which are also divisors of 2n.
 * https://oeis.org/A138652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of differences between consecutive divisors * of
 *            2n that themselves divide 2n. * * Algorithm: * 1. Compute m =
 *            2*n. * 2. Factorize m using factor_u64. * 3. Generate all
 *            divisors of m. * 4. Sort the divisors. * 5. Count how many
 *            consecutive differences divide m. * 6. Return that count
 * Range:     u8 n <= 405, u16 n <= 8476, u32 n <= 8476, u64 n <= 8476,
 *            u128 n <= 8476, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* function: void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Comparator for qsort on seq_t */
static int seq_cmp(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Static helper that returns the number of divisors (τ) of m given
 * its prime factorization in arrays primes[] and exponents[] of length cnt.
 */
static size_t divisor_count(size_t cnt, const uint64_t exponents[])
{
    size_t d = 1;
    for (size_t i = 0; i < cnt; ++i) {
        d *= (exponents[i] + 1);
    }
    return d;
}

/* The requested sequence function. */
seq_t a(idx_t n)
{
    /* 1. Compute 2*n (as uint64_t).  The sequence definition only
     * requires n >= 1; we assume n fits in 64 bits.
     */
    uint64_t n64 = (uint64_t)n;
    uint64_t m64 = n64 * 2u;
    seq_t m = (seq_t)m64;          /* used for division test */

    /* 2. Factor 2*n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64(m64, primes, exps, &prime_cnt);

    /* 3. Compute number of divisors. */
    size_t dcnt = divisor_count(prime_cnt, exps);

    /* 4. Generate all divisors. */
    seq_t *divs = (seq_t *)malloc(dcnt * sizeof(seq_t));
    if (!divs) return 0;          /* allocation failure – unlikely for small n */

    divs[0] = 1;
    size_t cur = 1;               /* current number of divisors */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t p_pow = 1;
        size_t prev = cur;
        for (int k = 1; k <= (int)exps[i]; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < prev; ++j) {
                divs[cur++] = divs[j] * (seq_t)p_pow;
            }
        }
    }

    /* 5. Sort the divisors in ascending order. */
    qsort(divs, dcnt, sizeof(seq_t), seq_cmp);

    /* 6. Count differences that divide m. */
    seq_t count = 0;
    for (size_t i = 0; i + 1 < dcnt; ++i) {
        seq_t diff = divs[i + 1] - divs[i];
        if (diff != 0 && (m % diff) == 0) {
            ++count;
        }
    }

    /* Clean up. */
    free(divs);

    return count;
}
