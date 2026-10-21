/*
 * A059412  Number of distinct minimal unary DFA's with exactly n states.
 * https://oeis.org/A059412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = psi(n) + Sum_{j=1}^{n-1} psi(n-j) * 2^(j-1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where psi(n) = Σ_{d|n} μ(d) * 2^(n/d)  (μ = Möbius function)   */

/*-----------------------------------------------------------------
 * Standard includes.  seq_t and idx_t are defined by the build.
 *-----------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/*-----------------------------------------------------------------
 * Global caches for powers of two and ψ-values.
 *-----------------------------------------------------------------*/
static idx_t pow2_cache_size = 0;
static seq_t *pow2_cache = NULL;     /* powers of two: pow2_cache[k] = 2^k   */

static idx_t psi_cache_size = 0;
static seq_t *psi_cache = NULL;      /* ψ-cache: psi_cache[n] = ψ(n)        */

/*-----------------------------------------------------------------
 * Ensure that pow2_cache contains 2^k for all k ≤ n.
 *-----------------------------------------------------------------*/
static void ensure_pow2(idx_t n)
{
    if (pow2_cache == NULL)          /* first use */
    {
        pow2_cache = (seq_t *)malloc(sizeof(seq_t));
        if (pow2_cache == NULL) exit(EXIT_FAILURE); /* allocation failure */
        pow2_cache[0] = (seq_t)1;
        pow2_cache_size = 0;
    }
    if (n <= pow2_cache_size) return;

    idx_t old = pow2_cache_size;
    idx_t new_sz = n + 1;
    seq_t *tmp = (seq_t *)realloc(pow2_cache, new_sz * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    pow2_cache = tmp;
    for (idx_t i = old + 1; i <= n; ++i)
        pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
    pow2_cache_size = n;
}

/*-----------------------------------------------------------------
 * Recursive helper to compute ψ(n).  The recursion enumerates all
 * square‑free divisors d of n; for each d it adds or subtracts
 * 2^(n/d) according to the Möbius function value.
 *-----------------------------------------------------------------*/
static void psi_rec(idx_t idx, idx_t curr_d, int mu_sign,
                    idx_t n, idx_t *primes, idx_t *exps,
                    int pcnt, seq_t *res)
{
    if (idx == pcnt)
    {
        idx_t exp = n / curr_d;                  /* n/d  */
        seq_t term = pow2_cache[exp];
        if (mu_sign > 0)
            *res += term;
        else
            *res -= term;
        return;
    }
    psi_rec(idx + 1, curr_d, mu_sign, n, primes, exps, pcnt, res);  /* skip prime */

    if (exps[idx] >= 1)                         /* include prime once */
    {
        idx_t new_d = curr_d * primes[idx];
        psi_rec(idx + 1, new_d, -mu_sign, n, primes, exps, pcnt, res);
    }
    /* multiples of the prime >1 give μ = 0 – no need to consider. */
}

/*-----------------------------------------------------------------
 * Compute ψ(n).  Assumes pow2_cache has been prepared for n.
 *-----------------------------------------------------------------*/
static seq_t psi_compute(idx_t n)
{
    idx_t tmp = n;
    idx_t primes[32];
    idx_t exps[32];
    int pcnt = 0;

    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
            }
            primes[pcnt] = p;
            exps[pcnt] = (idx_t)cnt;
            ++pcnt;
        }
    }
    if (tmp > 1)
    {
        primes[pcnt] = tmp;
        exps[pcnt] = (idx_t)1;
        ++pcnt;
    }

    seq_t result = (seq_t)0;
    if (n > 0)
        psi_rec(0, (idx_t)1, 1, n, primes, exps, pcnt, &result);
    return result;
}

/*-----------------------------------------------------------------
 * Ensure that ψ-cache contains values up to n.
 *-----------------------------------------------------------------*/
static void ensure_psi(idx_t n)
{
    ensure_pow2(n);                  /* powers of two first */

    if (psi_cache == NULL)           /* first use */
    {
        psi_cache = (seq_t *)malloc(sizeof(seq_t));
        if (psi_cache == NULL) exit(EXIT_FAILURE);
        psi_cache[0] = (seq_t)0;     /* unused dummy */
        psi_cache_size = 0;
    }
    if (n <= psi_cache_size) return;

    idx_t old = psi_cache_size;
    idx_t new_sz = n + 1;
    seq_t *tmp = (seq_t *)realloc(psi_cache, new_sz * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    psi_cache = tmp;
    for (idx_t i = old + 1; i <= n; ++i)
        psi_cache[i] = psi_compute(i);
    psi_cache_size = n;
}

/*-----------------------------------------------------------------
 * Main sequence function.
 *-----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n < 1 */

    ensure_psi(n);                 /* ψ(n) and all smaller ψ-values */

    seq_t sum = psi_cache[n];      /* the ψ(n) term */

    for (idx_t j = 1; j < n; ++j)
    {
        seq_t term = psi_cache[n - j];
        term *= pow2_cache[j - 1];
        sum += term;
    }
    return sum;
}
