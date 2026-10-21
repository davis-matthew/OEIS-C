/*
 * A087905  a(n) = n! * Sum_{d|n} (d/n)^d.
 * https://oeis.org/A087905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} (d/n)^d (offset 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n); */
#include "../utils/factor_u64.h"  /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

static void
gen_divisors_rec(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                 size_t pos, idx_t current, idx_t *divs, size_t *dcount)
{
    if (pos == cnt) {
        divs[*dcount] = current;
        ++*dcount;
        return;
    }
    uint64_t p = pr[pos];
    uint64_t e = ex[pos];
    idx_t val = current;
    /* exponent 0 */
    gen_divisors_rec(pr, ex, cnt, pos + 1, val, divs, dcount);
    /* exponents 1..e */
    for (uint64_t k = 1; k <= e; ++k) {
        val *= (idx_t)p;
        gen_divisors_rec(pr, ex, cnt, pos + 1, val, divs, dcount);
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) {
        return 0;   /* offset is 1; value before that is undefined */
    }

    /* Compute n! once */
    seq_t fact = factorial(n);

    /* Factorize n to get divisors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    if (n > 1) {
        factor_u64((uint64_t)n, primes, exps, &pf_cnt);
    }

    /* Generate all divisors of n */
    idx_t divs[2048];
    size_t dcnt = 0;
    gen_divisors_rec(primes, exps, pf_cnt, 0, (idx_t)1, divs, &dcnt);

    /* Sum over divisors d of n of n! / ( (n/d)^d ) */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d   = divs[i];
        idx_t m   = n / d;          /* m = n/d   */
        seq_t term = fact;          /* start with n! */
        for (idx_t k = 0; k < d; ++k) {
            term /= (seq_t)m;      /* divide by m exactly d times */
        }
        sum += term;
    }
    return sum;
}
