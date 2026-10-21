/*
 * A217364  a(n) = 2^n*binomial(5*n, n)/(4*n+1).
 * https://oeis.org/A217364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * binomial(5*n, n) / (4*n + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Static cache of primes up to the largest limit requested */
static uint64_t *primes = NULL;     /* array of primes */
static size_t   primes_cnt = 0;     /* number of primes in the array */
static idx_t    primes_limit = 0;   /* largest prime value pre‑computed */

/* Ensure that the primes array contains all primes ≤ limit */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;                      /* already sufficient */

    /* Allocate a new array of primes up to the requested limit */
    uint64_t *new_primes = NULL;
    size_t   new_cnt    = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* Replace the old array, freeing it if necessary */
    if (primes != NULL)
        free(primes);
    primes        = new_primes;
    primes_cnt    = new_cnt;
    primes_limit  = limit;
}

/* Factorise a small integer m into its prime exponents.
 * the factor list is returned in `p` and `exp`, with the number of
 * factors written to `cnt`.  The arrays must be large enough for the
 * maximum number of distinct factors (here 64 is more than enough). */
static void
factor_small(idx_t m, idx_t *p, idx_t *exp, int *cnt)
{
    *cnt = 0;
    idx_t temp = m;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t pp = primes[i];
        if (temp % pp == 0) {
            idx_t e = 0;
            while (temp % pp == 0) {
                temp /= pp;
                ++e;
            }
            p[(*cnt)]   = (idx_t)pp;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    /* handle a possible remaining prime factor > sqrt(m) */
    if (temp > 1) {
        p[(*cnt)]   = temp;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 */

    /* The denominator uses only primes ≤ 4n+1, which is ≤ 5n for n≥1 */
    idx_t limit = 5 * n;
    ensure_primes(limit);

    /* Factorise the denominator (4*n + 1) */
    idx_t denom_p[64];
    idx_t denom_exp[64];
    int    denom_cnt;
    factor_small(4 * n + 1, denom_p, denom_exp, &denom_cnt);

    seq_t result = (seq_t)1;           /* will accumulate the final value */

    /* For every prime p ≤ 5n compute its exponent in
     *  a(n) = 2^n * (5n)! / ((4n)! * n! * (4n+1))      */
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint64_t p_val = primes[i];
        idx_t    exp = 0;

        /* Legendre sums for v_p((5n)!) - v_p((4n)!) - v_p(n!) */
        idx_t power = p_val;
        while (power <= 5 * n) {
            exp += (5 * n) / power;
            power *= p_val;
        }
        power = p_val;
        while (power <= 4 * n) {
            exp -= (4 * n) / power;
            power *= p_val;
        }
        power = p_val;
        while (power <= n) {
            exp -= n / power;
            power *= p_val;
        }

        /* multiply by 2^n when p == 2 */
        if (p_val == 2)
            exp += n;

        /* subtract the exponent coming from the denominator factor (4n+1) */
        for (int j = 0; j < denom_cnt; ++j) {
            if (denom_p[j] == p_val) {
                if (exp > denom_exp[j])
                    exp -= denom_exp[j];
                else
                    exp = 0;         /* safety: the result is always an integer */
                break;
            }
        }

        if (exp > 0) {
            seq_t pow_p = seq_pow((seq_t)p_val, exp);
            result = seq_mul(result, pow_p);
        }
    }

    return result;
}
