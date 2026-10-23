/*
 * A162177  a(n) is the number of composite numbers that are smaller than
 *          A008578(n).
 * https://oeis.org/A162177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of composite integers < the (n‑1)th prime, i.e.
 *            a(n) = p_{n-1} – n. For n=1 the value is defined to be 0
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

/* -------------  Internal state ------------- */
static bool *prime_sieve = NULL;   /* sieve flags: true if prime */
static uint64_t *prime_lst = NULL;/* list of primes in ascending order*/
static size_t prime_cnt = 0;       /* number of primes stored      */
static idx_t sieve_limit   = 0;    /* upper bound of the sieve    */


/* -------------  Utility: build sieve ------------- */
static void build_sieve_up_to(idx_t limit)
{
    /* free existing data */
    free(prime_sieve);
    free(prime_lst);

    /* allocate sieve */
    prime_sieve = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!prime_sieve) {          /* allocation failed: nothing to do */
        sieve_limit = 0;
        return;
    }

    for (size_t i = 2; i <= (size_t)limit; ++i) prime_sieve[i] = true;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                prime_sieve[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (prime_sieve[i]) ++cnt;

    prime_lst = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_lst) {
        prime_cnt = 0;
        sieve_limit = 0;
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (prime_sieve[i]) prime_lst[idx++] = (uint64_t)i;

    prime_cnt   = cnt;
    sieve_limit = (size_t)limit;
}

/* -------------  Ensure we have enough primes ------------- */
static void ensure_sieve(idx_t n)
{
    /* we need at least the (n‑1)th prime */
    if (prime_cnt >= (size_t)(n - 1)) return;

    idx_t new_limit;
    if (n < 6) {
        new_limit = 15;                     /* enough for the first few primes */
    } else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn)));  /* upper bound for p_n */
        new_limit = (idx_t)(est + 10.0);              /* safety margin */
    }

    /* If the generated sieve is not large enough, double it */
    if (new_limit <= sieve_limit)
        new_limit = sieve_limit * 2;

    build_sieve_up_to(new_limit);
}

/* -------------  Main sequence function ------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* offset 1 – a(1)=0 by definition */

    ensure_sieve(n);               /* make sure we have prime_{n‑1} */

    /* prime_{n‑1} is stored at index n‑2 of prime_lst */
    seq_t prime_val = (seq_t)prime_lst[n - 2];
    return prime_val - (seq_t)n;   /* a(n) = p_{n‑1} – n */
}
