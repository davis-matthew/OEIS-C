/*
 * A092481  Number of different sets of n-gons labeled 1...n such that all
 *          members of each set contain equivalent paths with increasing
 *          labels; i.e., the number of isotemporal classes of n-gons.
 * https://oeis.org/A092481
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   If n odd: a(n) = (1/n) * sum_{d|n} (2^(n/d-1)-1) * φ(d). If n =
 *            4k+2 (n%4==2): a(n) = (1/n) * (sum_{d|n} 2^(n/d-1)φ(d) -
 *            sum_{c|n/2} 2^(n/2c-1)φ(2c)) + 2^((n-4)/2). If n = 4k (n%4==0):
 *            a(n) = (1/n) * (sum_{d|n} 2^(n/d-1)φ(d) - sum_{c|n/2}
 *            2^(n/2c-1)φ(2c)) + 2^((n-4)/2) + 2^((n-8)/4) - 2^(ceil((n-...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   build_primes, factor_uint64, pow2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/factor_uint64.h"
#include "../utils/pow2_idx.h"

/* Prime table for factorisation */
static idx_t  *PRIMES = NULL;
static size_t  PRIME_CNT = 0;
static bool    PRIMES_INIT = false;

static void init_primes(void)
{
    if (PRIMES_INIT) return;
    build_primes((idx_t)10000, (idx_t**)&PRIMES, &PRIME_CNT);
    PRIMES_INIT = true;
}

/* Euler’s totient for values up to 10000, using pre‑generated primes. */
static seq_t phi_u64(uint64_t n)
{
    seq_t res = (seq_t)n;
    uint64_t m = n;
    for (size_t i = 0; i < PRIME_CNT && PRIMES[i] * PRIMES[i] <= m; ++i) {
        uint64_t p = PRIMES[i];
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res -= res / (seq_t)p;
        }
    }
    if (m > 1) res -= res / (seq_t)m;
    return res;
}

/* Recursive generation of all divisors of a number obtained from its factorisation
   (prime array `pr`, exponent array `exp`, number of prime factors `num`).
   For each divisor d it adds φ(d) * 2^(n/d-1) to the accumulator *sum. */
static void dfs_sum1(int idx, uint64_t d, const uint64_t *pr, const int *exp,
                     int num, idx_t n, seq_t *sum)
{
    if (idx == num) {
        seq_t term = phi_u64(d) * pow2_idx(n / d - 1);
        *sum += term;
        return;
    }
    uint64_t p = pr[idx];
    int em = exp[idx];
    uint64_t d_cur = d;
    for (int e = 0; e <= em; ++e) {
        dfs_sum1(idx + 1, d_cur, pr, exp, num, n, sum);
        d_cur *= p;
    }
}

/* Same recursion but for divisors c of n/2. Adds φ(2c) * 2^( (n/2)/c -1 ) to *sum. */
static void dfs_sum2(int idx, uint64_t d, const uint64_t *pr, const int *exp,
                     int num, idx_t m, seq_t *sum)
{
    if (idx == num) {
        seq_t term = phi_u64(2 * d) * pow2_idx(m / d - 1);
        *sum += term;
        return;
    }
    uint64_t p = pr[idx];
    int em = exp[idx];
    uint64_t d_cur = d;
    for (int e = 0; e <= em; ++e) {
        dfs_sum2(idx + 1, d_cur, pr, exp, num, m, sum);
        d_cur *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined regime – return 0 */
    init_primes();

    seq_t sum1 = 0;
    seq_t sum2 = 0;

    /* factorisation of n */
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64((uint64_t)n, fac, exp, &cnt);
    dfs_sum1(0, 1, fac, exp, cnt, n, &sum1);

    if ((n & 1) == 0) {          /* even n : compute the second sum */
        uint64_t m = (uint64_t)n / 2;
        uint64_t fac2[20];
        int exp2[20];
        int cnt2 = 0;
        factor_uint64(m, fac2, exp2, &cnt2);
        dfs_sum2(0, 1, fac2, exp2, cnt2, m, &sum2);
    }

    if ((n & 1) == 1) {          /* odd n */
        seq_t tmp = sum1 - (seq_t)n;
        return tmp / (seq_t)n;
    } else if ((n % 4) == 2) {    /* n = 4k+2 */
        seq_t res = (sum1 - sum2) / (seq_t)n;
        seq_t extra = pow2_idx((idx_t)((n - 4) / 2));
        return res + extra;
    } else {                     /* n = 4k  */
        seq_t res = (sum1 - sum2) / (seq_t)n;
        seq_t extra1 = pow2_idx((idx_t)((n - 4) / 2));
        /* compute 2^((n-8)/4) only if exponent non‑negative */
        int e2 = (int)(n / 4) - 2;          /* (n-8)/4 */
        seq_t extra2 = 0;
        if (e2 >= 0) extra2 = pow2_idx((idx_t)e2);
        /* exponent ceil((n-4)/8)-1  ->  ((n+3)/8)-1   */
        int e3 = (int)((n + 3) / 8) - 1;
        seq_t extra3 = 0;
        if (e3 >= 0) extra3 = pow2_idx((idx_t)e3);
        return res + extra1 + extra2 - extra3;
    }
}
