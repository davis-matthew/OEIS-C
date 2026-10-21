/*
 * A098018  a(n) = Sum_{k|n, k>=2} mu(k-1), where mu() is the Moebius
 *          function.
 * https://oeis.org/A098018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k | n, k >= 2} mu(k-1) (OEIS A098018)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static tables for μ‑function and primes */
static int8_t  *mu_tbl   = NULL;   /* μ[0 .. mu_limit] */
static idx_t    mu_limit = 0;       /* highest index stored */

static int    *primes   = NULL;     /* primes up to current sieve limit */
static size_t  p_cnt    = 0;        /* number of primes stored */

/* Rebuild μ[0..limit] and the primes list.  The sieve is linear.  */
static void recompute_mu_and_primes(idx_t limit)
{
    if (limit <= mu_limit) return;          /* already large enough */

    /* Allocate or enlarge μ table */
    mu_tbl = (int8_t*)realloc(mu_tbl, (size_t)(limit + 1) * sizeof(int8_t));
    if (!mu_tbl) abort();
    mu_limit = limit;

    /* Allocate a buffer big enough for all primes up to `limit`. */
    primes = (int*)realloc(primes, (size_t)(limit + 1) * sizeof(int));
    if (!primes) abort();

    /* Linear sieve.  is_comp flags composites. */
    uint8_t *is_comp = (uint8_t*)calloc((size_t)limit + 1, 1);
    if (!is_comp) abort();

    mu_tbl[0] = 0;
    if (limit >= 1) mu_tbl[1] = 1;
    p_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[p_cnt++] = (int)i;
            mu_tbl[i]       = -1;          /* prime -> μ = −1 */
        }
        for (size_t j = 0; j < p_cnt; ++j) {
            idx_t mult = i * (idx_t)primes[j];
            if (mult > limit) break;
            is_comp[mult] = 1;
            if (i % primes[j] == 0) {
                mu_tbl[mult] = 0;          /* square factor → μ = 0 */
                break;
            } else {
                mu_tbl[mult] = -mu_tbl[i];
            }
        }
    }
    free(is_comp);
}

/* Recursive enumeration of all divisors of `n` given its prime factorization.
   Whenever a divisor `d >= 2` is finished, add μ(d-1) to *acc.  */
static void enum_divisors(size_t pos, idx_t cur, seq_t *acc,
                          const idx_t *p, const idx_t *e, size_t np)
{
    if (pos == np) {
        if (cur >= 2) *acc += (seq_t)mu_tbl[cur - 1];
        return;
    }
    idx_t prime = p[pos];
    idx_t exp   = e[pos];
    idx_t mult  = 1;
    for (int j = 0; j <= exp; ++j) {
        enum_divisors(pos + 1, cur * mult, acc, p, e, np);
        mult *= prime;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below offset */

    /* Build μ up to n‑1, because the largest divisor‐1 is n‑1. */
    idx_t need = (n > 0) ? (n - 1) : 0;
    recompute_mu_and_primes(need);

    /* Factor n by trial division using the precomputed primes. */
    idx_t primes_f[16];
    idx_t exps_f[16];
    size_t nf = 0;

    idx_t tmp = n;
    for (size_t i = 0; i < p_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            idx_t e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            primes_f[nf] = primes[i];
            exps_f[nf]   = e;
            ++nf;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        primes_f[nf] = tmp;
        exps_f[nf]   = 1;
        ++nf;
    }

    seq_t sum = 0;
    enum_divisors(0, 1, &sum, primes_f, exps_f, nf);
    return sum;
}
