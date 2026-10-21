/*
 * A099726  Sum of remainders of the n-th prime mod k, for k = 1,2,3,...,n.
 * https://oeis.org/A099726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1..n} (p mod k), where p = prime(n) (the n‑th prime)
 * Range:     u8 n <= 32, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc, abort */

static void ensure_primes(idx_t need);
static seq_t prime_n(idx_t n);

/* storage for primes generated lazily */
static seq_t *primes_arr   = NULL;   /* array of primes (seq_t)        */
static idx_t    primes_cnt = 0;      /* number of primes stored       */
static idx_t    primes_cap = 0;      /* capacity of the array         */

/* ------------------------------------------------------------------ */
/* Ensure that at least 'need' primes are available in primes_arr.    */
static void ensure_primes(idx_t need)
{
    if (primes_cnt >= need)
        return;

    /* Grow array capacity if necessary */
    if (primes_cap < need) {
        idx_t new_cap = primes_cap ? primes_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc((void *)primes_arr,
                                      new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failure – abort for safety */
            abort();
        primes_arr = tmp;
        primes_cap = new_cap;
    }

    /* Start candidate from 2 or the next integer after the last prime */
    seq_t candidate = (primes_cnt == 0) ? (seq_t)2 : primes_arr[primes_cnt - 1] + 1;

    /* Simple incremental primality test using the primes already known */
    while (primes_cnt < need) {
        int is_prime = 1;
        for (idx_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes_arr[i];
            /* stop once p*p > candidate */
            if (p > candidate / p)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            primes_arr[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Return the n-th prime (1‑based).  Returns 0 for n == 0. */
static seq_t prime_n(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_primes(n);
    return primes_arr[n - 1];
}

/* ------------------------------------------------------------------ */
/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* domain starts at n = 1 */

    seq_t p = prime_n(n);         /* n-th prime, as seq_t */

    seq_t sum_floor = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t q = p / (seq_t)k;          /* floor(p/k) */
        sum_floor += (seq_t)k * q;       /* k * floor(p/k) */
    }

    return (seq_t)n * p - sum_floor;     /* Σ (p mod k) */
}
