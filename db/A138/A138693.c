/*
 * A138693  Numbers of the form 110 + p^2. (where p is a prime).
 * https://oeis.org/A138693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 110 + p_n^2 (p_n = n-th prime, p_1 = 2)
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/* Prime table utilities                                               */
/* ------------------------------------------------------------------ */
static uint64_t *prime_tbl   = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;       /* number of primes stored */
static size_t   prime_cap   = 0;       /* allocated capacity */
static uint64_t next_candidate = 2;    /* next integer to test for primality */

/* Ensure that at least `need` primes are available in prime_tbl */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;

    /* Expand storage if required */
    if (prime_cap < (size_t)need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 256;
        if (new_cap < (size_t)need)
            new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
        if (!tmp)
            exit(1);                     /* failure */
        prime_tbl = tmp;
        prime_cap = new_cap;
    }

    /* Incrementally generate primes by trial division using the table itself */
    while (prime_cnt < (size_t)need) {
        if (is_prime(next_candidate))
            prime_tbl[prime_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Sequence definition                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence is defined for n >= 1 */
        return (seq_t)0;

    ensure_primes(n);                    /* make sure we have the nth prime */
    uint64_t p = prime_tbl[n - 1];       /* primes are 0‑based indexed */

    /* compute 110 + p^2 in the type seq_t */
    seq_t result = ((seq_t)p * (seq_t)p) + (seq_t)110;
    return result;
}
