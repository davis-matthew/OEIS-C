/*
 * A124219  a(n)= m-th positive divisor of n, where phi(n) is number of
 *          positive integers which are <= n and are coprime to n, d(n) is the
 *          number of positive divisors of n and m = d(n) if d(n)|phi(n), else
 *          m = phi(n) mod d(n).
 * https://oeis.org/A124219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the m‑th divisor of n, where m = d(n) if d(n) divides
 *            φ(n), otherwise m = φ(n) mod d(n)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   factor_u64, num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following utilities are provided by the build system. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/num_divisors.h"
#include "../utils/totient.h"

/* Generate all divisors of n from its prime factorisation.
   primes[i] ^ exps[i] are the prime powers. */
static void gen_divs_u(size_t idx, uint64_t cur,
                       const uint64_t *primes, const uint64_t *exps,
                       size_t cnt, uint64_t *divs, size_t *pos)
{
    if (idx == cnt) {
        divs[(*pos)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t cur_pow = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_u(idx + 1, cur * cur_pow,
                   primes, exps, cnt, divs, pos);
        cur_pow *= p;
    }
}

#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort on uint64_t values. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* Number of divisors */
    uint64_t d64 = num_divisors((uint64_t)n);
    seq_t D   = (seq_t)d64;            /* d(n) as seq_t */

    /* Euler's totient */
    seq_t P   = totient((idx_t)n);         /* φ(n) as seq_t */

    /* Compute m */
    seq_t m;
    if (P % D == 0)
        m = D;
    else
        m = P % D;

    /* Factor n into primes and exponents */
    size_t prime_cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Generate all divisors */
    size_t dcount = (size_t)d64;
    uint64_t *divs = (uint64_t *)malloc(dcount * sizeof(uint64_t));
    size_t pos = 0;
    gen_divs_u(0, 1ULL, primes, exps, prime_cnt, divs, &pos);

    /* Sort the divisors */
    qsort(divs, dcount, sizeof(uint64_t), cmp_uint64);

    /* Pick the m-th divisor (1‑based). */
    seq_t result = (seq_t)divs[(size_t)m - 1];

    free(divs);
    return result;
}
