/*
 * A169900  Earliest sequence such that xy | a(x+y) for all x>=1, y>=1.
 * https://oeis.org/A169900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = LCM_{1<=x<n} x*(n-x) (the minimal integer such that xy |
 *            a(x+y) for all x,y). Computation is done by prime factorisation
 *            of n as described in the OEIS entry
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mul_small.h"

/* Helper: compute integer floor log base p of m (m>=1, p>=2).
   Returns the largest e such that p^e <= m. */
static int floor_log_p(uint64_t p, uint64_t m)
{
    int e = 0;
    uint64_t t = 1;
    while (t <= m / p) {    /* avoid overflow */
        t *= p;
        ++e;
    }
    return e;
}

/* Global primes array */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_init = false;

/* Initialise global primes up to a fixed limit (sufficient for all calls). */
static void init_primes(void)
{
    if (primes_init) return;
    idx_t limit = 200000;          /* enough for n up to 200 000 (and comfortably >10 000) */
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_init = true;
}

/* Factorise n into prime powers.
   `fac` and `exp` are arrays of size at least 16.
   Returns the number of distinct primes. */
static int factor_u64(idx_t n, uint64_t fac[16], int exp[16])
{
    int cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            int e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            fac[cnt] = primes[i];
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        fac[cnt] = tmp;
        exp[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* Factorise n */
    uint64_t fac[16];
    int exp[16];
    int nf = factor_u64(n, fac, exp);

    /* Check whether n is a pure prime power */
    bool prime_power = (nf == 1);

    seq_t res = 1;

    if (prime_power) {
        uint64_t p = fac[0];
        int d   = exp[0];
        /* p^(2*d-2) */
        int e = 2 * d - 2;
        for (int i = 0; i < e; ++i)
            res = mul_small(res, (unsigned int)p);

        /* Other primes q < n (q != p) */
        for (size_t i = 0; i < primes_cnt && primes[i] < n; ++i) {
            uint64_t q = primes[i];
            if (q == p) continue;
            int exp_q = floor_log_p(q, n);
            for (int j = 0; j < exp_q; ++j)
                res = mul_small(res, (unsigned int)q);
        }
    } else {
        /* n^2 */
        res = mul_small((seq_t)n, (unsigned int)n);

        /* For each prime p < n */
        for (size_t i = 0; i < primes_cnt && primes[i] < n; ++i) {
            uint64_t p = primes[i];
            /* p-adic order of n */
            int nu = 0;
            for (int k = 0; k < nf; ++k) {
                if (fac[k] == p) {
                    nu = exp[k];
                    break;
                }
            }
            /* compute m = n / p^nu */
            uint64_t base_pow = 1;
            for (int k = 0; k < nu; ++k) base_pow *= p;
            uint64_t m = n / base_pow;

            int exp_p = floor_log_p(p, m);
            for (int j = 0; j < exp_p; ++j)
                res = mul_small(res, (unsigned int)p);
        }
    }

    return res;
}
