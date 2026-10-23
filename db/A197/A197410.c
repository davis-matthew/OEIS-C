/*
 * A197410  Product of cumulative sums of divisors of n.
 * https://oeis.org/A197410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = product of cumulative sums of divisors of n
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 35, u64 n <= 119, u128 n <= 359,
 *            big any n
 * Helpers:   factor_u64, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_one.h"

/* compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t ua = *(const uint64_t *)a;
    uint64_t ub = *(const uint64_t *)b;
    if (ua < ub) return -1;
    if (ua > ub) return 1;
    return 0;
}

/* recursive generation of all divisors of n
   pr   : array of prime factors
   ex   : corresponding exponents
   cnt  : number of distinct prime factors
   pos  : current index in pr/ex
   cur  : current divisor being built
   out  : output array for divisors
   outcnt: pointer to current number of divisors in out  */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t pos, uint64_t cur, uint64_t *out, size_t *outcnt)
{
    if (pos == cnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    uint64_t p = pr[pos];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        gen_divisors(pr, ex, cnt, pos + 1, val, out, outcnt);
        val *= p;
    }
}

/* Main function computing the sequence value */
seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t prime_cnt = 0;
    uint64_t tmp = (uint64_t)n;
    factor_u64(tmp, primes, exps, &prime_cnt);

    /* Generate all divisors */
    uint64_t divs[128];
    size_t div_cnt = 0;
    if (n == 0) {
        /* Undefined for n=0, return 0 */
        return (seq_t)0;
    }
    gen_divisors(primes, exps, prime_cnt, 0, 1ULL, divs, &div_cnt);

    /* Sort divisors */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Compute product of cumulative sums */
    seq_t product = seq_one();      /* start at 1 */
    seq_t cumulative = (seq_t)0;    /* start at 0 */

    for (size_t i = 0; i < div_cnt; ++i) {
        cumulative += (seq_t)divs[i];
        product *= cumulative;
    }

    return product;
}
