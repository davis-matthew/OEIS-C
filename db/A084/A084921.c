/*
 * A084921  a(n) = lcm(p-1, p+1) where p is the n-th prime.
 * https://oeis.org/A084921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(p-1, p+1) where p is the nth prime (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 71, u32 n <= 6941, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* ----- prime storage ------------------------------------------- */
static idx_t *prime_cache = NULL; /* primes[0] = 2, primes[1] = 3, … */
static size_t prime_cnt   = 0;     /* number of primes stored       */
static size_t prime_cap   = 0;     /* allocated capacity            */

/* expand the prime cache to hold at least 'min_cap' elements  */
static void expand_prime_cache(size_t min_cap)
{
    if (prime_cap >= min_cap)
        return;
    size_t new_cap = prime_cap > 0 ? prime_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
    if (!tmp)
        exit(EXIT_FAILURE);   /* abort on allocation failure    */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* add one prime to the cache */
static void add_prime(idx_t p)
{
    expand_prime_cache(prime_cnt + 1);
    prime_cache[prime_cnt++] = p;
}

/* test whether 'x' is prime using the cached primes */
static bool is_prime_candidate(idx_t x)
{
    if (x < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_cache[i];
        if (p > x / p)        /* p*p > x   */
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* ensure that prime_cache contains at least 'n' primes (n ≥ 1) */
static void ensure_prime_cnt(idx_t n)
{
    if (prime_cnt >= n)
        return;

    if (prime_cnt == 0)
        add_prime(2);

    idx_t cand = prime_cache[prime_cnt - 1] + 1;
    if (cand % 2 == 0)
        ++cand;          /* make it odd for speed */

    while (prime_cnt < n) {
        if (is_prime_candidate(cand))
            add_prime(cand);
        cand += 2;        /* skip even numbers   */
    }
}

/* --------------------------------------------------------------- */
/* Main sequence function                                          */
seq_t a(idx_t n)
{
    if (n == 0)           /* below offset – undefined; return 0      */
        return 0;

    ensure_prime_cnt(n);

    idx_t p = prime_cache[n - 1];  /* nth prime (1‑based) */
    if (p == 2)          /* special case: prime 2 */
        return (seq_t)3; /* lcm(1,3) = 3        */

    /* odd prime:  lcm(p-1, p+1) = (p²-1)/2 */
    seq_t p_seq = (seq_t)p;
    seq_t p2    = p_seq * p_seq;
    return (p2 - 1) / 2;
}
