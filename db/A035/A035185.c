/*
 * A035185  Number of divisors of n == 1 or 7 (mod 8) minus number of divisors
 *          of n == 3 or 5 (mod 8).
 * https://oeis.org/A035185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∑_{d|n} w(d) where w(d)=+1 if d≡1 or 7 (mod 8), w(d)=−1
 *            if d≡3 or 5 (mod 8), w(d)= 0 otherwise. a is multiplicative, and
 *            a(2^e) = 1 a(p^e) = e+1 if p≡1 or 7 (mod 8) a(p^e) =
 *            (1+(-1)^e)/2 = 1 if e even, 0 if e odd if p≡3 or 5 (mod 8)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle n=0 is not required (offset is 1). */
    if (n == 0) return 0;

    /* Temporary buffers for prime factors and exponents. */
    uint64_t pr[64];   /* prime factors */
    uint64_t ex[64];   /* exponents */
    size_t  cnt;       /* number of distinct prime factors */

    /* Factor n. The helper writes to pr[], ex[], cnt. */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = pr[i];
        uint64_t e   = ex[i];
        uint64_t mod = p & 7ULL;   /* p % 8 */

        if (mod == 2ull) {
            /* a(2^e) = 1, so no effect on the product */
            continue;
        }
        else if (mod == 1ull || mod == 7ull) {
            /* a(p^e) = e + 1 */
            result *= (seq_t)(e + 1);
        }
        else /* mod == 3 or 5 */ {
            /* a(p^e) = 1 if e even, 0 otherwise */
            if (e & 1ULL)
            {
                /* Multiplicative factor 0 → entire result is 0 */
                return (seq_t)0;
            }
            /* else factor is 1, nothing to multiply */
        }
    }

    return result;
}
