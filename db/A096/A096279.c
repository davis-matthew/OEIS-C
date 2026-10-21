/*
 * A096279  Sums of successive sums of successive sums of successive sums of
 *          successive primes.
 * https://oeis.org/A096279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binom(4,0)*p(n) + binom(4,1)*p(n+1) + binom(4,2)*p(n+2) +
 *            binom(4,3)*p(n+3) + binom(4,4)*p(n+4)
 * Range:     u8 n <= 4, u16 n <= 562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where p(k) is the k‑th prime (p(1)=2).  The function generates the
    required primes lazily and then returns the weighted sum. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------- Prime table ----------------------------------------- */
static uint64_t *prime_tbl = NULL;   /* array of primes                 */
static size_t   prime_cnt = 0;       /* number of primes stored         */
static size_t   prime_cap = 0;       /* allocated capacity in primes     */

/* Ensure that prime_tbl holds at least 'need' primes (1‑based).     */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Grow capacity if necessary. */
    if (prime_cap < need) {
        size_t ncap = (prime_cap ? prime_cap * 2 : need);
        if (ncap < need)
            ncap = need;
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl, ncap * sizeof(uint64_t));
        if (!tmp)
            abort();
        prime_tbl = tmp;
        prime_cap = ncap;
    }

    /* If table empty, seed first two primes 2 and 3. */
    if (prime_cnt == 0) {
        prime_tbl[prime_cnt++] = 2;
        if (prime_cnt >= need)
            return;
    }

    /* Fill primes by trial division. */
    uint64_t candidate = (prime_cnt == 1 ? 3 : prime_tbl[prime_cnt - 1] + 2);
    if ((candidate & 1) == 0)  /* keep odd numbers only */
        ++candidate;

    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_tbl[j];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            prime_tbl[prime_cnt++] = candidate;
        candidate += 2;     /* next odd candidate */
    }
}

/* --------------------------------------------------------------- */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1, but guard */

    /* We need primes up to index n+4 (1‑based). */
    size_t need = (size_t)n + 4;
    ensure_primes(need);

    /* Compute weighted sum using binomial coefficients 1,4,6,4,1. */
    size_t i = (size_t)n - 1;           /* 0‑based index of p(n) */
    seq_t sum = (seq_t)prime_tbl[i] +            /* 1 * p(n)   */
               (seq_t)4 * prime_tbl[i + 1] +    /* 4 * p(n+1) */
               (seq_t)6 * prime_tbl[i + 2] +    /* 6 * p(n+2) */
               (seq_t)4 * prime_tbl[i + 3] +    /* 4 * p(n+3) */
               (seq_t)1 * prime_tbl[i + 4];     /* 1 * p(n+4) */

    return sum;
}
