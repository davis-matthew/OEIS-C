/*
 * A180851  Sum of increasing powers of divisors: a(n) = Sum_{i=1..q} d(i)^i
 *          where d(1) < d(2) < ... < d(q) are the divisors of n.
 * https://oeis.org/A180851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i=1..q} d(i)^i (divisors of n in increasing order)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 59, u128 n <= 179,
 *            big any n
 * Helpers:   big_pow, qsort_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses trial‑division factorization and the big_pow helper from utils. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/qsort_uint64.h"

static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) {
    size_t c = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t exp = 0;
            while (n % d == 0) {
                n /= d;
                exp++;
            }
            pr[c] = d;
            ex[c] = exp;
            c++;
        }
        d = (d == 2) ? 3 : d + 2;
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        c++;
    }
    *cnt = c;
}

static void gen_divs(size_t idx, uint64_t cur,
                     const uint64_t *pr, const uint64_t *ex, size_t npr,
                     uint64_t *divs, size_t *dcnt) {
    if (idx == npr) {
        divs[(*dcnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = cur;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        gen_divs(idx + 1, val, pr, ex, npr, divs, dcnt);
        val *= p;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for OEIS, guard */
    uint64_t primes[64], exps[64];
    size_t np = 0;
    factor_uint64((uint64_t)n, primes, exps, &np);

    /* Max divisor count for n up to 10^4 is small; use a fixed buffer */
    uint64_t divisors[256];
    size_t dcnt = 0;
    gen_divs(0, 1, primes, exps, np, divisors, &dcnt);

    qsort_uint64(divisors, dcnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t base = (seq_t)divisors[i];
        seq_t exp  = (seq_t)(i + 1);
        sum += big_pow(base, exp);
    }
    return sum;
}
