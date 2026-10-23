/*
 * A136528  a(n) = the highest possible number of positive divisors of the sum
 *          of any two distinct positive divisors of n.
 * https://oeis.org/A136528
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ d(x + y) : x,y positive divisors of n, x<y }
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

/* Generate all divisors of n from its prime factorisation.
   primes[] and ex[] hold the primes and their exponents,
   tot is the number of distinct primes. */
static void gen_divs(const uint64_t *primes, const uint64_t *ex,
                     int idx, int tot,
                     uint64_t curr, uint64_t *divs, size_t *cnt)
{
    if (idx == tot) {
        divs[(*cnt)++] = curr;
        return;
    }
    uint64_t p = primes[idx];
    int emax = (int)ex[idx];
    uint64_t val = curr;
    for (int e = 0; e <= emax; ++e) {
        gen_divs(primes, ex, idx + 1, tot, val, divs, cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n<2, return 0 */

    uint64_t N = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pfcnt = 0;

    /* Factorise N.  The helper writes the prime factors into primes[]
       and their exponents into exps[] and sets pfcnt. */
    factor_u64(N, primes, exps, &pfcnt);

    /* Generate all divisors of N.  For n≤10000 at most ~100 are needed. */
    uint64_t divs[256];
    size_t dcnt = 0;
    gen_divs(primes, exps, 0, (int)pfcnt, 1, divs, &dcnt);

    /* Compute the maximum divisor count among sums of distinct pairs. */
    size_t maxd = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        for (size_t j = i + 1; j < dcnt; ++j) {
            uint64_t sum = divs[i] + divs[j];
            size_t dsum = (size_t)divisor_count_u64(sum);
            if (dsum > maxd) maxd = dsum;
        }
    }
    return (seq_t)maxd;
}
