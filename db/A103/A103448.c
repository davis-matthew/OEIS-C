/*
 * A103448  a(n) = Sum_{k=0..n} Moebius(binomial(n,k)).
 * https://oeis.org/A103448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} mu(binomial(n,k)) (Moebius of the binomial
 *            coefficient)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   legendre_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/legendre_exp.h"

/* Function prototype expected by the build */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/*      Prime table (static, shared by all calls)                        */
/* --------------------------------------------------------------------- */
static int  *prime_list = NULL;   /* primes ≤ current max_n */
static size_t  prime_cnt  = 0;     /* number of stored primes   */
static idx_t   max_prime  = 0;     /* largest n for which table is valid */

/* Simple sieve: fill prime_list with all primes ≤ limit (inclusive) */
static void sieve_primes(idx_t limit)
{
    if (limit <= max_prime) return;

    size_t sz = (size_t)limit + 1;
    char *is_prime = (char *)malloc(sz);
    if (!is_prime) exit(EXIT_FAILURE);

    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= (size_t)limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = 0;

    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++count;

    int *new_list = (int *)malloc(count * sizeof(int));
    if (!new_list) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) new_list[idx++] = (int)i;

    free(is_prime);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = count;
    max_prime  = limit;
}

/* --------------------------------------------------------------------- */
/*               Main sequence function                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Prepare prime table up to n */
    sieve_primes(n);

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        int ones = 0;      /* number of primes with exponent exactly 1 */
        int squarefree = 1;/* becomes 0 if any prime has exponent >1 */

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            int p = prime_list[pi];
            int exp = legendre_exp(n, p) - legendre_exp(k, p) - legendre_exp(n - k, p);
            if (exp == 0) continue;
            if (exp > 1) { squarefree = 0; break; }
            /* exp == 1 */
            ones ^= 1;          /* toggle parity */
        }

        if (squarefree) {
            if (ones == 0) sum += (seq_t)1;
            else           sum += (seq_t)(-1);
        }
    }
    return sum;
}
