/*
 * A066978  a(n) = gcd(prime(2*n)+1, prime(n)+1).
 * https://oeis.org/A066978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(prime(2*n)+1 , prime(n)+1) (offset 1)
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64, int_est_va_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd_u64.h"
#include "../utils/int_est_va_limit.h"

/* ---------  state for prime generation  --------- */
static idx_t  sieve_limit   = 0;   /* last limit used for sieve */
static bool  *is_comp      = NULL;/* sieve array: true=composite */
static idx_t *prime_list   = NULL;/* prime_list[i] = (i+1)-th prime */
static size_t prime_count  = 0;    /* number of primes stored */

/* ---------- small helper: 64‑bit gcd ------------- */

/* ---------  ensure we have at least 'need' primes  --------- */
static void
ensure_primes(idx_t need)
{
    if (prime_count >= need)
        return;

    /* determine new sieve limit */
    idx_t new_limit = int_est_va_limit(need * 1.1); /* some safety margin */

    /* allocate and perform sieve */
    bool *new_comp = (bool*)calloc((size_t)new_limit + 1, sizeof(bool));
    if (!new_comp)
        return;   /* unlikely to happen for the limits of the problem */

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (!new_comp[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_comp[j] = true;
        }
    }

    /* gather primes */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!new_comp[i]) ++new_cnt;

    /* allocate new prime list */
    idx_t *new_list = (idx_t*)malloc(new_cnt * sizeof(idx_t));
    if (!new_list) {
        free(new_comp);
        return;   /* unlikely */
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!new_comp[i]) new_list[pos++] = i;

    /* free old data */
    free(is_comp);
    free(prime_list);

    /* store new data */
    is_comp       = new_comp;
    prime_list    = new_list;
    prime_count   = new_cnt;
    sieve_limit   = new_limit;
}

/* ------- return the k-th prime (1‑based) -------- */
static inline uint64_t
nth_prime_u64(idx_t k)
{
    if (k < 1) return 0;
    ensure_primes(k);
    return (uint64_t)prime_list[k - 1];
}

/* ---------  public sequence function  --------- */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1; safe default */

    /* we need prime(n) and prime(2*n) */
    uint64_t p_n  = nth_prime_u64(n);
    uint64_t p_2n = nth_prime_u64(n * 2ULL);

    uint64_t val1 = p_n  + 1ULL;
    uint64_t val2 = p_2n + 1ULL;

    uint64_t g = gcd_u64(val1, val2);

    return (seq_t)g;
}
