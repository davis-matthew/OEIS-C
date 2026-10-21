/*
 * A074657  Number of Lyndon words (aperiodic necklaces) with 5n beads of 5
 *          colors, n beads of each color.
 * https://oeis.org/A074657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(5n) * Σ_{d|n} μ(n/d) * (5d)! / (d!⁵) where μ is the
 *            Möbius function. The term for n=0 is defined to be 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t n, idx_t k) */

/* Recursive enumeration of divisors of n.
   Arguments:
     idx      : current index in the prime factor list
     cur      : current divisor value
     n        : original n
     primes   : array of prime factors of n
     exps     : corresponding exponents of those primes in n
     pcnt     : number of distinct prime factors
     cur_exp  : array of exponents of the current divisor
     sumPos   : sum of terms with μ(m) = +1
     sumNeg   : sum of terms with μ(m) = -1
*/
static void dfs_div(int idx, uint64_t cur, uint64_t n,
                    uint64_t *primes, uint64_t *exps, int pcnt,
                    uint64_t *cur_exp, seq_t *sumPos, seq_t *sumNeg)
{
    if (idx == pcnt) {
        /* d = cur, m = n / d */
        int mu = 0;          /* 0 means μ(m)=0, 1 for +1, -1 for -1 */
        int parity = 0;
        int zero = 0;
        for (int j = 0; j < pcnt; ++j) {
            uint64_t mExp = exps[j] - cur_exp[j];
            if (mExp > 1) { zero = 1; break; }
            if (mExp == 1) parity ^= 1;
        }
        if (zero) return;
        mu = (parity == 0) ? 1 : -1;

        /* (5d)! / (d!⁵) = binom(5d,d)*binom(4d,d)*binom(3d,d)*binom(2d,d) */
        seq_t term = binom(5 * cur, cur);
        term *= binom(4 * cur, cur);
        term *= binom(3 * cur, cur);
        term *= binom(2 * cur, cur);

        if (mu > 0) *sumPos += term;
        else        *sumNeg += term;
        return;
    }

    uint64_t val = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        cur_exp[idx] = e;
        dfs_div(idx + 1, cur * val, n, primes, exps, pcnt,
                cur_exp, sumPos, sumNeg);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Factorise n */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    int pcnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t c = 0;
            while (tmp % p == 0) { tmp /= p; ++c; }
            primes[pcnt] = p;
            exps[pcnt]  = c;
            ++pcnt;
        }
    }
    if (tmp > 1 && pcnt < 64) {
        primes[pcnt] = tmp;
        exps[pcnt]  = 1;
        ++pcnt;
    }

    uint64_t cur_exp[64] = {0};
    seq_t sumPos = 0, sumNeg = 0;

    dfs_div(0, 1, (uint64_t)n, primes, exps, pcnt, cur_exp,
            &sumPos, &sumNeg);

    seq_t total = sumPos - sumNeg;           /* always non‑negative */
    seq_t denom = 5 * (seq_t)n;              /* 5*n fits in idx_t */
    return total / denom;
}
