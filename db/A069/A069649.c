/*
 * A069649  Let M_n be the n X n matrix with M_n(i,j)=i^3/(i+j); then
 *          a(n)=1/det(M_n).
 * https://oeis.org/A069649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = \prod_{i=1}^n \prod_{j=1}^n (i+j) / \Bigl((n!)^3
 *            \,\prod_{1\le i<j\le n}(j-i)^2\Bigr)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   seq_pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by prime‑factorisation using the above
   closed form.  For each prime p we accumulate an exponent e(p) and
   finally build the result as \prod p^{e(p)}.  The intermediate
   exponent array is signed because intermediate cancellations can
   produce negative values, but the final result is integral, so all
   exponents are non‑negative.  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow_seq_t.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, offset is 1 */

    /* work with native integer for small sizes (n <= 20000000 is safe) */
    size_t N = (size_t)n;
    size_t limit = 2 * N;          /* largest i+j */

    /* build smallest prime factor (spf) array up to limit */
    uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) return 0;            /* allocation failed */
    spf[0] = 0; spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }

    /* exponent array for primes, indexed by value */
    int32_t *exps = (int32_t *)calloc(limit + 1, sizeof(int32_t));
    if (!exps) { free(spf); return 0; }

    /* ----- 1. product over all sums (i+j) ----- */
    for (size_t s = 2; s <= limit; ++s)
    {
        size_t count = (s <= N + 1) ? (s - 1) : (2 * N + 1 - s);
        size_t tmp = s;
        while (tmp > 1)
        {
            uint32_t p = spf[tmp];
            uint32_t cnt = 0;
            while (tmp % p == 0) { tmp /= p; ++cnt; }
            exps[p] += (int32_t)(cnt * count);
        }
    }

    /* ----- 2. product of squared differences (j-i)^2 ----- */
    for (size_t d = 1; d <= N - 1; ++d)
    {
        size_t mul = 2 * (N - d);   /* multiplicity of d^2 */
        size_t tmp = d;
        while (tmp > 1)
        {
            uint32_t p = spf[tmp];
            uint32_t cnt = 0;
            while (tmp % p == 0) { tmp /= p; ++cnt; }
            exps[p] -= (int32_t)(cnt * mul);
        }
    }

    /* ----- 3. subtract 3 * exponent of n! ----- */
    for (size_t p = 2; p <= limit; ++p)
    {
        if (spf[p] != p) continue;   /* not prime */
        if (p > N) break;
        uint64_t pp = p;
        uint32_t efact = 0;
        while (pp <= N) { efact += (uint32_t)(N / pp); pp *= p; }
        exps[p] -= 3 * (int32_t)efact;
    }

    /* ----- 4. build the result from prime powers ----- */
    seq_t result = 1;
    for (size_t p = 2; p <= limit; ++p)
    {
        if (spf[p] != p) continue;   /* not prime */
        int32_t e = exps[p];
        if (e > 0) result *= seq_pow_seq_t((seq_t)p, (idx_t)e);
        /* e should never be negative for a correct integer result */
    }

    free(spf);
    free(exps);
    return result;
}
