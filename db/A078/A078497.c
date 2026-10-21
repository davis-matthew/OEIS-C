/*
 * A078497  The member r of a triple of primes (p,q,r) in arithmetic
 *          progression which sum to 3*prime(n) = A001748(n) = p + q + r.
 * https://oeis.org/A078497
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + d where d is the smallest positive integer
 *            such that prime(n)-d and prime(n)+d are both prime. (The triple
 *            {p,q,r} is an AP of primes with q=prime(n) and r=q+d.)
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------- includes --------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------- sieve ------------------------------------ */
#define SIEVE_LIMIT 2000000ULL          /* covers nth prime up to about 10⁵     */
#define PRIME_ARRAY_CAP 200000          /* > 148933 primes below 2 M           */

static unsigned char *prime_flag = NULL;     /* 0 = prime, 1 = composite         */
static unsigned int prime_list[PRIME_ARRAY_CAP];
static size_t prime_count = 0;

/* initialize the sieve and the prime list once */
static void init_primes(void)
{
    if (prime_count != 0)                /* already done */
        return;

    prime_flag = (unsigned char *)malloc((SIEVE_LIMIT + 1) * sizeof(unsigned char));
    if (!prime_flag)
        exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= SIEVE_LIMIT; ++i)
        prime_flag[i] = 0;

    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!prime_flag[i]) {
            if (prime_count < PRIME_ARRAY_CAP)
                prime_list[prime_count++] = (unsigned int)i;
            if (i * i <= SIEVE_LIMIT) {
                for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    prime_flag[j] = 1;
            }
        }
    }
}

/* --------------------------- sequence function ----------------------- */
seq_t a(idx_t n)
{
    if (n < 3)                         /* offset is 3; below is undefined */
        return (seq_t)0;

    init_primes();

    /* nth prime (1‑based) */
    uint64_t q = (uint64_t)prime_list[n - 1];

    /* find smallest d>=1 so that q-d and q+d are primes */
    for (uint64_t d = 1; q > d + 1; ++d) {
        if (q - d <= SIEVE_LIMIT && prime_flag[q - d])   /* not prime -> skip */
            continue;
        if (q + d <= SIEVE_LIMIT && prime_flag[q + d])   /* not prime -> skip */
            continue;
        return (seq_t)(q + d);       /* r = q + d */
    }

    /* should never happen for n up to 10 000 with this sieve limit */
    return (seq_t)0;
}
