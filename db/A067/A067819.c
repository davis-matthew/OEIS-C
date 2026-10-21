/*
 * A067819  Sum of the divisors of binomial(2n,n).
 * https://oeis.org/A067819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ( binomial(2n,n) ) (sum of the divisors of the central
 *            binomial coefficient)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* static cache of primes up to the current limit */
static idx_t *prime_list = NULL;   /* array of primes up to sieve_limit */
static size_t  prime_cnt  = 0;     /* number of primes stored */
static idx_t   sieve_lim  = 0;     /* maximal number up to which primes are known */

/* enlarge the prime list to cover all primes ≤ limit, using a simple sieve */
static void maybe_sieve(idx_t limit)
{
    if (limit <= sieve_lim) return;   /* already sufficient */

    /* allocate a flag array that marks composites */
    bool *is_comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp) return;             /* allocation failed – keep old state */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[(size_t)j] = true;

    /* count primes and build a fresh list */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i]) ++new_cnt;

    idx_t *new_pr = (idx_t *)malloc(new_cnt * sizeof(idx_t));
    if (!new_pr) {
        free(is_comp);
        return;                       /* allocation failed – keep old state */
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i]) new_pr[pos++] = i;

    /* replace the old cache */
    free(prime_list);
    prime_list = new_pr;
    prime_cnt  = new_cnt;
    sieve_lim  = limit;

    free(is_comp);
}

/* the required OEIS sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) not defined in OEIS; keep for safety */

    idx_t two_n = 2 * n;
    maybe_sieve(two_n);            /* make sure we have all primes ≤ 2n */

    seq_t sigma = (seq_t)1;        /* product of (p^(e+1)-1)/(p-1) for all primes */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = prime_list[pi];
        if (p > two_n) break;

        /* exponents of p in n!, (2n)! using Legendre's formula */
        idx_t power = p;
        int    e     = 0;
        while (power <= two_n) {
            e += (int)(two_n / power) - 2 * (int)(n / power);
            if (power > two_n / p) break;     /* avoid overflow on next step */
            power *= p;
        }
        if (e <= 0) continue;

        /* accumulate (1 + p + p^2 + ... + p^e) into sigma */
        seq_t term = (seq_t)1;   /* start with p^0 */
        seq_t cur  = (seq_t)1;   /* current p^i */
        for (int i = 1; i <= e; ++i) {
            cur *= (seq_t)p;      /* next power of p */
            term += cur;          /* add it to the divisor‑sum for p */
        }
        sigma *= term;
    }

    return sigma;
}
