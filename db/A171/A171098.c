/*
 * A171098  Solutions to the equation M(n) = -3 (M = Mertens's function
 *          A002321).
 * https://oeis.org/A171098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that the Mertens function M(k) =
 *            -3, where M(k) = Σ_{i=1..k} μ(i) and μ is the Möbius function.
 *            The function returns the k corresponding to the nth solution,
 *            with a(1) = 13
 * Range:     u8 n <= 44, u16 n <= 1222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Global state for memoisation */
static idx_t       *solutions   = NULL; /* array of found k values (1‑based)   */
static size_t         sol_cnt   = 0;    /* number of solutions stored         */
static size_t         sol_cap   = 0;    /* capacity of solutions array        */

static idx_t       last_limit = 0;      /* largest n for which we have computed M(n)   */

/* Rebuilds mu and solutions arrays up to the given limit.
   All previous data are discarded. */
static void compute_up_to(idx_t limit)
{
    /* free previous arrays if any */
    if (solutions != NULL) {
        free(solutions);
        solutions = NULL;
        sol_cnt   = 0;
        sol_cap   = 0;
    }

    if (limit < 1) {
        last_limit = 1;
        return;
    }

    size_t L = (size_t)limit;  /* cast to size_t for array indices */

    /* Allocate arrays for least prime factor (lp) and μ values (mu). */
    uint32_t *lp = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    int8_t   *mu = (int8_t *)malloc((L + 1) * sizeof(int8_t));
    if (!lp || !mu) {
        free(lp); free(mu);
        return; /* allocation failed – nothing to do */
    }

    memset(lp, 0, (L + 1) * sizeof(uint32_t));
    mu[0] = 0;
    mu[1] = 1;
    for (size_t i = 2; i <= L; ++i) mu[i] = 1;

    /* List of primes: we grow this vector as needed. */
    size_t primes_cap = L / 10 + 10;            /* initial capacity guess   */
    uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) {
        free(lp); free(mu);
        return;
    }
    size_t primes_cnt = 0;

    /* Linear sieve to compute μ up to L. */
    for (size_t i = 2; i <= L; ++i) {
        if (lp[i] == 0) {
            lp[i] = (uint32_t)i;
            if (primes_cnt >= primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t pj = 0; pj < primes_cnt; ++pj) {
            uint32_t p = primes[pj];
            if (p > lp[i]) break;
            size_t ip = i * (size_t)p;
            if (ip > L) break;
            lp[ip] = p;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    free(lp);

    /* Scan through 1..limit to collect solutions where M(k) == -3. */
    int acc = 0; /* running Mertens sum */
    for (idx_t i = 1; i <= limit; ++i) {
        acc += mu[(size_t)i];
        if (acc == -3) {
            if (sol_cnt == sol_cap) {
                sol_cap = sol_cap ? sol_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(solutions, sol_cap * sizeof(idx_t));
                if (!tmp) break;
                solutions = tmp;
            }
            solutions[sol_cnt++] = i;
        }
    }

    free(mu);
    free(primes);
    last_limit = limit;
}

/* Expands known solutions until at least 'needed' ones are available. */
static void ensure_solutions(idx_t needed)
{
    if (sol_cnt >= (size_t)needed) return;

    idx_t next_limit = last_limit ? last_limit * 2 : 100000;
    /* Cap to avoid overflow; if too big, use maximum representable value. */
    if (next_limit < last_limit) next_limit = (idx_t)-1;

    while (sol_cnt < (size_t)needed && next_limit != (idx_t)-1) {
        compute_up_to(next_limit);
        if (sol_cnt >= (size_t)needed) break;
        /* Prepare for next expansion */
        if (next_limit > ((idx_t)-1) / 2) next_limit = (idx_t)-1;
        else next_limit *= 2;
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of bounds – not used */
    ensure_solutions(n);
    return (seq_t)solutions[n - 1];
}
