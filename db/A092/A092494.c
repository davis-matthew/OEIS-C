/*
 * A092494  a(n) = Sum_{p prime and p<=n} ceiling(n/p).
 * https://oeis.org/A092494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p prime, p≤n} ⌈n/p⌉ (offset 1)
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Prime sieve (static storage)                                         */
/* --------------------------------------------------------------------- */
static idx_t      sieve_limit = 0;     /* maximum n for which primes are known */
static idx_t     *prime_arr   = NULL;  /* array of primes (stored as idx_t)   */
static size_t    prime_cnt  = 0;       /* number of primes found              */
static size_t    prime_cap  = 0;       /* current capacity of prime_arr       */

/* Extend the sieve so that all primes ≤ limit are present in prime_arr. */
static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;            /* already ready */

    /* Allocate a boolean array for the sieve */
    size_t sz = (size_t)(limit + 1);
    bool *comp = (bool *)malloc(sz);
    if (!comp)
        return;            /* out‑of‑memory – caller will see zero result */

    for (size_t i = 0; i < sz; ++i)
        comp[i] = false;

    /* Allocate enough space for the primes (rough upper bound). */
    size_t needed = (size_t)(limit / 10 + 10);
    if (prime_cap < needed) {
        idx_t *tmp = (idx_t *)realloc(prime_arr, needed * sizeof(idx_t));
        if (!tmp) {
            free(comp);
            return;
        }
        prime_arr = tmp;
        prime_cap = needed;
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[(size_t)i]) {        /* i is prime */
            if (cnt >= prime_cap) {   /* expand the prime array if necessary */
                prime_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(prime_arr, prime_cap * sizeof(idx_t));
                if (!tmp) {
                    free(comp);
                    return;
                }
                prime_arr = tmp;
            }
            prime_arr[cnt++] = i;      /* store as idx_t */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    comp[(size_t)j] = true;
            }
        }
    }
    prime_cnt   = cnt;
    sieve_limit = limit;
    free(comp);
}

/* --------------------------------------------------------------------- */
/*  Sequence A092494                                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)                /* a(1)=0; no primes ≤ n for n<2 */
        return 0;

    ensure_primes(n);         /* make sure we have all primes ≤ n */

    seq_t total = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_arr[i];     /* convert idx_t prime to seq_t */
        if (p > (seq_t)n)
            break;
        /* ceil(n/p) = (n + p - 1) / p */
        seq_t t = ((seq_t)n + p - 1) / p;
        total += t;
    }
    return total;
}
