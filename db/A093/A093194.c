/*
 * A093194  Minimal values of m=a^2+b^2=c^2+d^2 for each x=a+b+c+d=6*p (p=any
 *          odd prime).
 * https://oeis.org/A093194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 * (p_n^2 + 1), where p_n is the nth odd prime * (p_1 =
 *            3, p_2 = 5, p_3 = 7, …). The implementation generates primes *
 *            on demand using a simple incremental sieve. All arithmetic is *
 *            performed in the type seq_t, which may be any unsigned integer
 *            type * (uint8_t … uint64_t, unsigned __int1...
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Cache of primes: prime_values[0] == 2, prime_values[1] == 3, … */
static uint64_t *prime_values   = NULL;
static size_t   prime_cnt       = 0;    /* number of primes stored      */
static size_t   prime_cap       = 0;    /* allocated capacity           */
static uint64_t current_limit   = 0;    /* highest number sieved so far */

/* Ensure that we have at least (n+1) primes stored (index n is valid).
   If not, extend the sieve until sufficient primes have been found. */
static void
ensure_primes_upto(idx_t n)
{
    /* First initialisation */
    if (prime_values == NULL) {
        prime_cap = 1024;                            /* initial capacity */
        prime_values = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_values) exit(1);                  /* abort on OOM     */
        prime_values[0] = 2;                         /* first prime      */
        prime_cnt       = 1;
        current_limit   = 2;
    }

    /* Extend until we have at least n+1 primes (index n is needed) */
    while (prime_cnt <= (size_t)n) {
        double dn = (double)(n + 1);                /* +1 to be safe    */
        uint64_t new_limit;
        if (dn < 6.0)
            new_limit = 15;
        else {
            double estimate = dn * (log(dn) + log(log(dn)));
            new_limit = (uint64_t)(estimate + 10.0);
        }
        if (new_limit <= current_limit)
            new_limit = current_limit * 2;           /* double at least */

        /* Sieve up to new_limit */
        char *isprime = (char*)malloc(new_limit + 1);
        if (!isprime) exit(1);
        memset(isprime, 1, new_limit + 1);
        isprime[0] = isprime[1] = 0;
        for (uint64_t i = 2; i * i <= new_limit; ++i)
            if (isprime[i])
                for (uint64_t j = i * i; j <= new_limit; j += i)
                    isprime[j] = 0;

        /* Append new primes to the cache */
        for (uint64_t i = current_limit + 1; i <= new_limit; ++i)
            if (isprime[i]) {
                if (prime_cnt >= prime_cap) {
                    prime_cap *= 2;
                    prime_values = (uint64_t*)realloc(prime_values,
                                                     prime_cap * sizeof(uint64_t));
                    if (!prime_values) exit(1);
                }
                prime_values[prime_cnt++] = i;
            }

        free(isprime);
        current_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                /* defined for n>=1, placeholder for n==0 */
        return 0;

    ensure_primes_upto(n);

    /* nth odd prime is stored at index n (prime_values[0] = 2) */
    uint64_t p = prime_values[(size_t)n];

    /* Compute 5 * (p^2 + 1) using seq_t arithmetic */
    seq_t pt   = (seq_t)p;
    seq_t prod = pt * pt;                 /* p^2 */
    seq_t res  = 5 * (prod + 1);

    return res;
}
