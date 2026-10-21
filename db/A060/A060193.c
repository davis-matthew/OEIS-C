/*
 * A060193  Partial products of A060191.
 * https://oeis.org/A060193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the primes whose 1‑based index is ≡1 or 4 (mod
 *            4), i.e. the primes p₁, p₄, p₅, p₈, p₉, p₁₂, … . a(0)=2,
 *            a(1)=2·7=14, a(2)=2·7·11=154, …
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static storage for primes */
static uint64_t *prime_list      = NULL;   /* array of primes (1‑based indices) */
static size_t   prime_count      = 0;      /* number of primes in prime_list  */
static uint64_t prime_limit      = 0;      /* current upper bound of sieve      */

/* sieve of Eratosthenes up to `limit`, store all primes found in prime_list */
static void sieve_upto(uint64_t limit)
{
    /* allocate boolean array */
    char *is_prime = (char *)malloc((size_t)limit + 1);
    if (!is_prime) return;   /* allocation failed – should never happen */
    memset(is_prime, 1, (size_t)limit + 1);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (uint64_t i = 2; i <= limit / i; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes and allocate array */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;   /* allocation failed – should never happen */
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i])
            new_list[idx++] = i;
    }

    /* replace old list */
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    prime_limit = limit;

    free(is_prime);
}

/* ensure that we have at least `needed` primes in prime_list   */
static void ensure_primes(idx_t needed)
{
    if (prime_list == NULL) {
        prime_limit = 20;
        sieve_upto(prime_limit);
    }
    /* double the limit until we have enough primes */
    while (prime_count < needed) {
        sieve_upto(prime_limit * 2);
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* number of terms to multiply (including the first) */
    idx_t m = n + 1;
    /* we need at most 2*m primes (indices 1 and 4 modulo 4) */
    idx_t needed_primes = 2 * m;
    ensure_primes(needed_primes);

    seq_t prod = 1;
    for (idx_t i = 0; i < m; ++i) {
        /* compute the 1‑based index of the prime to use */
        idx_t prime_index;
        if ((i & 1) == 0)                      /* even i : 1 + 4*(i/2) */
            prime_index = 1 + 4 * (i / 2);
        else                                   /* odd  i : 4 + 4*((i-1)/2) */
            prime_index = 4 + 4 * ((i - 1) / 2);

        /* convert to 0‑based array index */
        uint64_t prime_value = prime_list[prime_index - 1];
        prod *= (seq_t)prime_value;
    }
    return prod;
}
