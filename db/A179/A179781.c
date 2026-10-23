/*
 * A179781  a(n) = AP(n) is the total number of aperiodic k-palindromes of n,
 *          1 <= k <= n.
 * https://oeis.org/A179781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} μ(n/d) * 2^{⌊d/2⌋}
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 65, u64 n <= 129, u128 n <= 200,
 *            big any n
 * Helpers:   factor_u64, mobius, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/pow2.h"
#include "../utils/factor_u64.h"

/* Recursively generate all divisors of n from its factorisation. */
static void
gen_div(size_t idx, idx_t cur,
        const uint64_t *primes, const uint64_t *exps,
        size_t pcount, idx_t *divs, size_t *dcnt)
{
    if (idx == pcount) {
        divs[(*dcnt)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    idx_t prod = cur;
    int limit = (int)exps[idx];
    for (int e = 0; e <= limit; ++e) {
        gen_div(idx + 1, prod, primes, exps, pcount, divs, dcnt);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defensive, the sequence is for n≥1 */

    /* 64‑bit numbers can have at most 64 distinct prime factors. */
    const size_t MAXP = 64;
    uint64_t primes[MAXP];
    uint64_t exps[MAXP];
    size_t pcount = 0;

    /* factorisation of n */
    factor_u64((uint64_t)n, primes, exps, &pcount);

    /* collect all divisors into array */
    idx_t divs[128];
    size_t dcnt = 0;
    gen_div(0, 1, primes, exps, pcount, divs, &dcnt);

    /* compute the Möbius‑weighted sum */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        idx_t q = n / d;                    /* n/d */
        int mu = (int)mobius(q);             /* μ(n/d) ∈ {-1,0,1} */
        if (mu) {
            seq_t term = pow2(d >> 1);      /* 2^{⌊d/2⌋} */
            if (mu > 0)
                sum += term;
            else
                sum -= term;
        }
    }
    return sum;
}
