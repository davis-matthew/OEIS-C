/*
 * A109301  a(n) = rhig(n) = rote height in gammas of n, where the "rote"
 *          corresponding to a positive integer n is a graph derived from the
 *          primes factorization of n, as illustrated in the comments.
 * https://oeis.org/A109301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rhig(n) where rhig(1) = 0, rhig(n) = 1 + max{
 *            rhig(index_of_prime(p)), rhig(e) } over all prime powers p^e in
 *            the factorisation of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------- */
/*  Includes and external types                  */
/* -------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/* `seq_t` and `idx_t` are provided by the build system. */

/* -------------------------------------------- */
/*  Prime list management (cached)              */
/* -------------------------------------------- */
static idx_t *prime_list = NULL;   /* array of primes, 0‑based */
static size_t  prime_count = 0;    /* number of primes stored   */
static uint64_t max_prime_limit = 0; /* last limit we built up to */

/* Build the prime list up to at least 'limit'.  The list is only
   expanded when a larger limit is requested, otherwise the existing
   list is left unchanged. */
static void build_prime_list(uint64_t limit)
{
    if (limit <= max_prime_limit)
        return;

    /* Allocate/expand the prime array. */
    size_t new_cap = (size_t)limit + 64;   /* a safety margin */
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);
    prime_list = tmp;

    /* Simple sieve of Eratosthenes up to 'limit'. */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_prime) exit(1);
    for (size_t i = 2; i <= (size_t)limit; ++i)
        is_prime[i] = 1;
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Store primes in the array. */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i])
            prime_list[cnt++] = (idx_t)i;

    prime_count = cnt;
    max_prime_limit = limit;

    free(is_prime);
}

/* Return the 1‑based index of a prime 'p' in the pre‑computed prime
   array.  The function assumes that 'p' is prime and that the prime
   list already contains all primes up to at least 'p'. */
static idx_t prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (idx_t)(mid + 1);
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid - 1;
    }
    return (idx_t)0;   /* Fallback – should never happen for a prime. */
}

/* -------------------------------------------- */
/*  Recursive computation of rhig(n)            */
/* -------------------------------------------- */
static seq_t rhig_rec(idx_t x)
{
    if (x <= 1) return (seq_t)0;

    /* Ensure the prime list contains all primes up to 'x'. */
    build_prime_list(x);

    /* Factorise 'x'.  The numbers below are small (<=x), so 64‑bit
       arithmetic suffices. */
    idx_t temp = x;
    idx_t primes[64];
    idx_t exps[64];
    size_t fac_cnt = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
        if (temp % prime_list[i] == 0) {
            idx_t p = prime_list[i];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[fac_cnt] = p;
            exps[fac_cnt]   = e;
            ++fac_cnt;
        }
    }
    if (temp > 1) {    /* Remaining prime factor. */
        primes[fac_cnt] = temp;
        exps[fac_cnt]   = 1;
        ++fac_cnt;
    }

    /* Compute the maximum of rhig(index_of_prime(p)) and rhig(e)
       over all factors. */
    seq_t max_val = (seq_t)0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        seq_t val_p = rhig_rec(prime_index(primes[i]));
        seq_t val_e = rhig_rec(exps[i]);
        if (val_p > max_val) max_val = val_p;
        if (val_e > max_val) max_val = val_e;
    }

    return max_val + (seq_t)1;
}

/* -------------------------------------------- */
/*  Public sequence function                   */
/* -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)   /* behaviour below offset is irrelevant */
        return (seq_t)0;
    return rhig_rec(n);
}
