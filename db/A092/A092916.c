/*
 * A092916  a(n) = A092914(n)/n = the least integer value of (n-1)!/(n*k!).
 * https://oeis.org/A092916
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (n-1)! / ( n * k! ) where k is the largest integer such
 *            that n * k! | (n-1)! . (a(n)=0 for prime n; the formula yields
 *            that automatically.) This implementation computes the maximal k
 *            by prime‑factor limits and then evaluates the quotient by
 *            multiplying only the necessary factors and cancel...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   fact_prime_exp, gcd, max_k_with_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/fact_prime_exp.h"
#include "../utils/max_k_with_exp.h"

/* Factorise n into distinct primes and their exponents.
    For n ≤ 10000 a simple trial division is fast enough.  */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t i = 0;
    while (n % 2 == 0) {
        if (i == 0 || primes[i - 1] != 2) {
            primes[i] = 2;
            exps[i]   = 0;
            i++;
        }
        exps[i - 1]++;
        n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (i == 0 || primes[i - 1] != p) {
                primes[i] = p;
                exps[i]   = 0;
                i++;
            }
            while (n % p == 0) {
                exps[i - 1]++;
                n /= p;
            }
        }
    }
    if (n > 1) {
        primes[i] = n;
        exps[i]   = 1;
        i++;
    }
    *cnt = i;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;          /* behaviour below offset not required */

    /* factor n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pf_cnt = 0;
    factor_uint64((uint64_t)n, primes, exps, &pf_cnt);

    /* maximal possible k is n-1 */
    idx_t max_k = (idx_t)(n - 1);

    /* For every prime dividing n, restrict k using exponent bounds */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p  = primes[i];
        seq_t   vp = (seq_t)exps[i];

        seq_t fact_exp = fact_prime_exp((idx_t)(n - 1), p);
        if (fact_exp < vp)                 /* n does not divide (n-1)! */
            return (seq_t)0;

        seq_t limit = fact_exp - vp;       /* allowed exponent in k! */
        idx_t k_p   = max_k_with_exp((idx_t)p, limit, max_k);
        if (k_p < max_k)
            max_k = k_p;
        if (max_k == 0)
            return (seq_t)0;                /* cannot reduce further */
    }

    /* Compute result = (n-1)! / (n * max_k!) safely */
    seq_t n_rem = (seq_t)n;
    seq_t res   = (seq_t)1;
    for (idx_t i = max_k + 1; i <= n - 1; ++i) {
        seq_t cur = (seq_t)i;
        seq_t g   = gcd(cur, n_rem);
        if (g > 1) {
            cur   /= g;
            n_rem /= g;
        }
        res *= cur;
    }
    /* any remaining factor of n after cancellation */
    if (n_rem > (seq_t)1)
        res /= n_rem;

    return res;
}
