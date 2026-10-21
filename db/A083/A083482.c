/*
 * A083482  Square root of smallest square of the type n(n+1)*k.
 * https://oeis.org/A083482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏(p^⌈e/2⌉) over the prime factorization p^e of n(n+1)
 * Range:     u8 n <= 20, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Trial‑division factorization for 64‑bit values.
   Accommodates up to MAX_FACTORS distinct prime factors. */
static size_t factorize(uint64_t n, uint64_t *primes, int *exps)
{
    size_t cnt = 0;
    if (n == 0) return 0;

    uint64_t tmp = n;
    if ((tmp & 1ULL) == 0) {               /* factor 2 */
        int e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            e++;
        }
        primes[cnt] = 2;
        exps[cnt] = e;
        ++cnt;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if ((tmp % p) == 0) {
            int e = 0;
            while ((tmp % p) == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
            if (cnt == 64) break;   /* safety for very few factors */
        }
    }
    if (tmp > 1 && cnt < 64) {           /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* value outside the OEIS offset */

    /* m = n * (n + 1)  (fits in 64 bits for n ≤ 10 000) */
    uint64_t m = (uint64_t)n * ((uint64_t)n + 1);

    uint64_t fac_primes[64];
    int fac_exps[64];
    size_t fac_cnt = factorize(m, fac_primes, fac_exps);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_primes[i];
        int e = fac_exps[i];
        int q = (e + 1) / 2;                 /* ceil(e/2) */
        seq_t pw = seq_pow((seq_t)p, (idx_t)q);
        result = seq_mul(result, pw);
    }
    return result;
}
