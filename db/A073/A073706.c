/*
 * A073706  a(n) = Sum_{ d divides n } (n/d)^(3d).
 * https://oeis.org/A073706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} (n/d)^(3*d) (n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


/* Fast exponentiation: base^exp with seq_t arithmetic. */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Trial‑division factorisation of n (uint64_t) into prime‑factor arrays.
   pr[0..cnt-1] holds primes, exp[0..cnt-1] their exponents. */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            pr[*cnt] = p;
            exp[*cnt] = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp[*cnt];
            }
            ++(*cnt);
        }
    }
    if (n > 1)
    {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* Recursive enumeration of all divisors from the prime‑factor arrays. */
static void dfs_div(const uint64_t *pr, const int *exp, size_t pc,
                    idx_t n, idx_t cur, seq_t *sum)
{
    if (pc == 0)
    {
        idx_t d = cur;
        idx_t m = n / d;
        seq_t term = pow_seq_t((seq_t)m, (idx_t)(3 * d));
        *sum = *sum + term;
        return;
    }
    const uint64_t p = pr[pc - 1];
    idx_t factor = 1;
    for (int e = 0; e <= exp[pc - 1]; ++e)
    {
        dfs_div(pr, exp, pc - 1, n, cur * factor, sum);
        factor *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1U) return 0;          /* undefined for n < 1 */
    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs_div(primes, exps, cnt, n, 1, &sum);
    return sum;
}
