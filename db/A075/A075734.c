/*
 * A075734  Fibonacci numbers F(k) when k is a product of an even number of
 *          distinct primes A030229 (mu(k)=1).
 * https://oeis.org/A075734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fib(k_n) where k_n is the nth integer with μ(k)=1 (i.e.,
 *            the product of an even number of distinct primes). The sequence
 *            is indexed starting at a(1)=Fib(1). This implementation builds
 *            the list of indices k_n by a linear sieve to compute Möbius
 *            values. It computes Fibonacci numbers by the p...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/fib.h"   /* provides: seq_t fib(idx_t) */

/* ------------------------------------------------------------------ */
/* Globals for caching the list of k values (n with μ(n)=1).           */
/* ------------------------------------------------------------------ */
static uint64_t *k_list   = NULL;   /* array of indices k_n          */
static size_t   k_size   = 0;        /* current number of terms stored*/
static size_t   k_cap    = 0;        /* allocated capacity of k_list  */
static uint64_t cur_limit = 0;       /* smallest limit for which we have computed μ  */

/* ------------------------------------------------------------------ */
/* Recompute μ up to `limit` and build the list of k_n.                 */
/* ------------------------------------------------------------------ */
static void recompute_to(uint64_t limit) {
    if (limit <= cur_limit) return;       /* nothing to do */

    /* Allocate arrays for μ and linear sieve */
    int *mu  = (int *)malloc((limit + 1) * sizeof(int));
    int *lp  = (int *)malloc((limit + 1) * sizeof(int));
    /* Rough upper bound for number of primes up to limit */
    size_t prime_cap = limit / 10 + 10;
    int *pr = (int *)malloc(prime_cap * sizeof(int));
    size_t pr_cnt = 0;

    /* Linear sieve: compute μ[n], lp[n] */
    mu[1] = 1;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (lp[i] == 0) {               /* i is prime */
            lp[i] = (int)i;
            if (pr_cnt < prime_cap) pr[pr_cnt++] = (int)i;
            mu[i] = -1;                 /* odd number of prime factors so far */
        }
        for (size_t j = 0; j < pr_cnt; ++j) {
            uint64_t p = (uint64_t)pr[j];
            if (p > (uint64_t)lp[i] || i * p > limit) break;
            lp[i * p] = (int)p;
            if (p == (uint64_t)lp[i])   /* square factor -> μ = 0 */
                mu[i * p] = 0;
            else
                mu[i * p] = -mu[i];
        }
    }

    /* Build the list of indices k with μ(k)=1 (including k=1) */
    size_t estimated = (size_t)(limit * 0.32) + 10;   /* density ≈ 30% */
    uint64_t *new_k = (uint64_t *)malloc(estimated * sizeof(uint64_t));
    size_t new_k_sz = 0;

    if (mu[1] == 1) new_k[new_k_sz++] = 1;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (mu[i] == 1) {
            if (new_k_sz >= estimated) {           /* grow if needed */
                estimated *= 2;
                new_k = (uint64_t *)realloc(new_k, estimated * sizeof(uint64_t));
            }
            new_k[new_k_sz++] = i;
        }
    }

    /* Clean up previous data and store new list */
    free(mu);
    free(lp);
    free(pr);
    free(k_list);

    k_list = new_k;
    k_size = new_k_sz;
    k_cap  = estimated;
    cur_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset starts at 1 */

    /* Estimate a safe upper bound for μ calculation */
    uint64_t need_limit = (uint64_t)n * 4 + 100;   /* 4×n is comfortably > 3.33×n */
    recompute_to(need_limit);

    /* Ensure we have at least n terms, else extend */
    while (k_size < (size_t)n) {
        recompute_to(cur_limit * 2 + 100);
    }

    /* Return the Fibonacci number at index k_n */
    return fib(k_list[n - 1]);
}
