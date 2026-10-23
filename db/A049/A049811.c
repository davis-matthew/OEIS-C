/*
 * A049811  a(n)=number of Farey fractions of order n that are <=1/7; cf.
 *          A049805.
 * https://oeis.org/A049811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{d=1..n} μ(d) · S(⌊n/d⌋) where S(m) = Σ_{k=1..m}
 *            ⌊k/7⌋
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   sum_floor_div7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_floor_div7.h"

/* External integer types */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------
 *  Möbius μ(1..N) with linear sieve
 * ------------------------------------------------------------------ */
static int8_t   *mu      = NULL;       /* μ[0..] */
static bool     *is_comp = NULL;       /* is_composite[0..] */
static uint64_t *primes  = NULL;       /* list of primes */
static size_t     primes_cnt  = 0;      /* number of primes found */
static idx_t      mu_nmax  = 0;        /* highest value computed */

static void build_mu(idx_t limit)
{
    if (limit <= mu_nmax)
        return;                        /* already computed up to limit */

    /* Allocate / extend μ array */
    int8_t  *new_mu   = (int8_t  *)realloc(mu,   (size_t)(limit + 1) * sizeof(int8_t));
    bool    *new_comp = (bool    *)realloc(is_comp, (size_t)(limit + 1) * sizeof(bool));
    if (!new_mu || !new_comp)
        abort();                       /* allocation failed */

    mu      = new_mu;
    is_comp = new_comp;

    /* Zero the newly allocated part (indices > old limit) */
    if (limit > mu_nmax) {
        memset(mu + mu_nmax + 1, 0,
               (size_t)(limit - mu_nmax) * sizeof(int8_t));
        memset(is_comp + mu_nmax + 1, 0,
               (size_t)(limit - mu_nmax) * sizeof(bool));
    }

    /* Allocate / extend primes array */
    if (!primes || primes_cnt < limit) {
        uint64_t *new_primes = (uint64_t *)realloc(primes,
                (size_t)(limit + 1) * sizeof(uint64_t));
        if (!new_primes)
            abort();
        primes = new_primes;
    }

    /* Re‑compute μ for 1..limit using linear sieve */
    mu[0] = 0;
    mu[1] = 1;
    primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t ip = i * primes[j];
            if (ip > limit)
                break;
            is_comp[ip] = true;
            if (i % primes[j] == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    mu_nmax = limit;
}

/* ------------------------------------------------------------------
 *  Main function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* OEIS offset starts at n = 1 */

    build_mu(n);

    int64_t sum = 0;           /* signed accumulator */

    for (idx_t d = 1; d <= n; ++d) {
        int8_t md = mu[d];
        if (md == 0)
            continue;
        seq_t s = sum_floor_div7(n / d);
        sum += (int64_t)md * (int64_t)s;
    }

    return (seq_t)(1 + sum);   /* +1 for the fraction 0/1 */
}
