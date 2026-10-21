/*
 * A217576  a(n) = Sum_{d divides n} (d!)^(n/d).
 * https://oeis.org/A217576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d divides n} (d!)^(n/d)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

/* Recursive helper to generate all divisors of n from its factorization. */
static void
gen_divisors_rec(idx_t idx, idx_t current,
                 const idx_t *primes, const size_t *exps,
                 size_t pcount, idx_t *divs, size_t *cnt)
{
    if (idx == pcount) {
        divs[*cnt] = current;
        ++*cnt;
        return;
    }
    idx_t p = primes[idx];
    idx_t mult = 1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors_rec(idx + 1, current * mult,
                         primes, exps, pcount, divs, cnt);
        mult *= p;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;  /* safety; offset starts at 1 */

    /* Factorize n into primes and exponents. */
    idx_t primes[16];
    size_t exps[16];
    size_t cnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Generate all divisors of n. */
    idx_t divs[512];
    size_t divc = 0;
    gen_divisors_rec(0, 1, primes, exps, cnt, divs, &divc);

    /* Compute the sum over divisors. */
    seq_t result = 0;
    for (size_t i = 0; i < divc; ++i) {
        idx_t d = divs[i];
        seq_t fact_d = factorial(d);
        idx_t exp = n / d;
        seq_t term = seq_pow(fact_d, exp);
        result += term;
    }
    return result;
}
