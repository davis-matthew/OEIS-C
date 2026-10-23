/*
 * A025560  a(n) = LCM{1, C(n-1,1), C(n-2,2), ..., C(n-[ n/2 ],[ n/2 ])}.
 * https://oeis.org/A025560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM{1, C(n-1,1), C(n-2,2), …, C(n-[n/2],[n/2])}
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* free */
#include "../utils/build_primes.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* ---------- helper ------------------------------------------- */
/* exponent of prime p in m! (Legendre's formula) */
static inline idx_t exp_in_fact(idx_t m, idx_t p)
{
    idx_t e = 0;
    for (idx_t pp = p; pp <= m; pp *= p)
        e += m / pp;
    return e;
}

/* ---------- static cache ------------------------------------- */
static idx_t   *prime_list   = NULL;  /* primes ≤ current limit */
static size_t  prime_cnt     = 0;     /* number of primes in prime_list */
static idx_t   prime_limit   = 0;     /* maximum n for which prime_list is valid */

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1, return 0 for safety */
        return 0;

    /* Ensure we have a prime list up to n */
    if (prime_list == NULL || prime_limit < n) {
        if (prime_list != NULL)
            free(prime_list);          /* discard old list */

        build_primes(n, &prime_list, &prime_cnt);
        prime_limit = n;                /* prime_list now valid for this n */
    }

    const idx_t max_k = n / 2;           /* floor(n/2) */
    seq_t lcm_val = 1;                   /* start with 1, lcm(1, *) = * */

    /* For each prime ≤ n, determine the maximum exponent that appears in
       any binomial C(n-k, k) with 1 ≤ k ≤ max_k, and multiply the result
       by that prime raised to that exponent. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        const idx_t p = prime_list[pi];
        idx_t max_exp = 0;

        for (idx_t k = 1; k <= max_k; ++k) {
            /* exponent of p in C(n-k, k)
               = exp_in_fact(n-k,p) - exp_in_fact(k,p) - exp_in_fact(n-2k,p)   */
            const idx_t e = exp_in_fact(n - k, p)
                          - exp_in_fact(k, p)
                          - exp_in_fact(n - 2 * k, p);

            if (e > max_exp)
                max_exp = e;
        }

        if (max_exp > 0) {
            const seq_t powp = seq_pow((seq_t)p, max_exp);
            lcm_val = seq_mul(lcm_val, powp);
        }
    }

    return lcm_val;
}
