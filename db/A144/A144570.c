/*
 * A144570  Nonprime(prime(n)).
 * https://oeis.org/A144570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A141468(A000040(n))
 * Range:     u8 n <= 46, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Helper: trial‑division primality test                                 */
/* --------------------------------------------------------------------- */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2) return false;
    for (idx_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/*  Primes cache: dynamic array of discovered primes                     */
/* --------------------------------------------------------------------- */
static idx_t *prime_cache      = NULL;
static size_t prime_cache_cap  = 0;
static size_t prime_cache_cnt  = 0;
static idx_t  prime_cache_next = 2;   /* next integer to test for primality */

/* Grow primes cache to at least `new_cap` elements */
static void grow_prime_cache(void)
{
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
    idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
    if (!tmp) abort();  /* allocation failed */
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

/* Ensure that prime_cache contains at least `need` primes. Returns
   the nth prime (1‑based). */
static idx_t get_prime_n(idx_t need)
{
    if (prime_cache_cnt >= (size_t)need)
        return prime_cache[need - 1];

    /* Extend cache until we have `need` primes. */
    while (prime_cache_cnt < (size_t)need) {
        if (is_prime_candidate(prime_cache_next)) {
            if (prime_cache_cnt == prime_cache_cap)
                grow_prime_cache();
            prime_cache[prime_cache_cnt++] = prime_cache_next;
        }
        ++prime_cache_next;
    }
    return prime_cache[need - 1];
}

/* --------------------------------------------------------------------- */
/*  Non‑prime cache: dynamic array of the first `size` non‑prime numbers.*/
/* --------------------------------------------------------------------- */
static idx_t *nonprime_cache      = NULL;
static size_t nonprime_cache_cap  = 0;
static size_t nonprime_cache_cnt  = 0;
static idx_t  nonprime_next       = 1;   /* next integer to evaluate */

/* Grow non‑prime cache to at least `new_cap` elements */
static void grow_nonprime_cache(void)
{
    size_t new_cap = nonprime_cache_cap ? nonprime_cache_cap * 2 : 16;
    idx_t *tmp = (idx_t *)realloc(nonprime_cache, new_cap * sizeof(idx_t));
    if (!tmp) abort();  /* allocation failed */
    nonprime_cache = tmp;
    nonprime_cache_cap = new_cap;
}

/* Ensure that nonprime_cache contains at least `need` non‑prime numbers. */
static void ensure_nonprimes(idx_t need)
{
    while (nonprime_cache_cnt < (size_t)need) {
        if (nonprime_next == 1) {
            /* 1 occurs twice */
            size_t toadd = (nonprime_cache_cnt + 1 < (size_t)need) ? 2 : 1;
            for (size_t i = 0; i < toadd; ++i) {
                if (nonprime_cache_cnt == nonprime_cache_cap)
                    grow_nonprime_cache();
                nonprime_cache[nonprime_cache_cnt++] = 1;
            }
            nonprime_next = 2;
            continue;
        }
        if (!is_prime_candidate(nonprime_next)) {
            if (nonprime_cache_cnt == nonprime_cache_cap)
                grow_nonprime_cache();
            nonprime_cache[nonprime_cache_cnt++] = nonprime_next;
        }
        ++nonprime_next;
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for n<1 */

    /* Step 1: find the n‑th prime number */
    idx_t prime_n = get_prime_n(n);

    /* Step 2: ensure we have at least prime_n non‑prime numbers */
    ensure_nonprimes(prime_n);

    /* Step 3: return the prime_n‑th non‑prime (1‑based) */
    return (seq_t)nonprime_cache[prime_n - 1];
}
