/*
 * A203858  a(n) = (1/n) * Sum_{d|n} moebius(n/d) * Lucas(d)^7, where Lucas(n)
 *          = A000204(n).
 * https://oeis.org/A203858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} μ(n/d) * Lucas(d)^7 where Lucas(n) =
 *            A000204(n) = 2*F(n+1) – F(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   factor_u64, fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/fib_pair.h"

/* Compute Lucas(n) via Fibonacci fast doubling */
static seq_t lucas_n(idx_t n)
{
    seq_t Fn, Fn1;
    fib_pair(n, &Fn, &Fn1);          /* Fn = F(n), Fn1 = F(n+1) */
    return Fn1 + Fn1 - Fn;           /* 2*F(n+1) – F(n) */
}

/* Recursively enumerate all divisors d of n.
   mu(n/d) is kept as (+1/-1 or 0) via parity and square */
static void dfs_mu_lucas(const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, size_t pos, idx_t d,
                         uint8_t parity, uint8_t square, seq_t *sum)
{
    if (pos == cnt) {
        if (!square) {
            seq_t l = lucas_n(d);
            /* Compute l^7 (multiplication is exact for seq_t) */
            seq_t t = l * l;          /* l^2 */
            t *= l;                   /* l^3 */
            t *= l;                   /* l^4 */
            t *= l;                   /* l^5 */
            t *= l;                   /* l^6 */
            t *= l;                   /* l^7 */
            if ((parity & 1) == 0)
                *sum += t;            /* μ = +1 */
            else
                *sum -= t;            /* μ = -1 */
        }
        return;
    }

    idx_t mul = 1;
    uint64_t e = ex[pos];
    for (uint64_t b = 0; b <= e; ++b) {
        idx_t nd = d * mul;
        uint64_t rem = e - b;           /* exponent in (n/d) for this prime */
        uint8_t npar = parity;
        uint8_t nsq = square;
        if (rem >= 2) {
            nsq = 1;                    /* μ becomes 0 */
        } else if (rem == 1) {
            npar ^= 1;
        }
        dfs_mu_lucas(pr, ex, cnt, pos + 1, nd, npar, nsq, sum);
        mul *= pr[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t fac_cnt = 0;
    factor_u64(n, primes, exponents, &fac_cnt);

    seq_t sum = 0;
    dfs_mu_lucas(primes, exponents, fac_cnt,
                 0, (idx_t)1, 0, 0, &sum);

    /* Exact division by n */
    sum /= n;
    return sum;
}
