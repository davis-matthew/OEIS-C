/*
 * A064661  a(n) = n - floor( prime(n)/log(prime(n)) ).
 * https://oeis.org/A064661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - floor( prime(n) / log( prime(n) ) ) (offset = 1)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ---------- prime cache: incremental sieve (linear) ---------- */
static uint64_t *prime_cache   = NULL;   /* array of primes          */
static size_t   prime_cap     = 0;       /* allocated capacity       */
static size_t   prime_cnt     = 0;       /* number of primes stored  */

/* grow the cache to at least `need` primes */
static void
ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;                               /* already have them */

    /* initialise lazily */
    if (!prime_cache) {
        prime_cap   = 64;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache)
            return;                           /* memory failure – abort */
        prime_cache[0] = 2;
        prime_cnt      = 1;
    }

    /* use the linear sieve algorithm to extend the prime list */
    size_t  i = prime_cnt;                   /* next candidate index   */
    size_t  limit_est = 0;                   /* will be set later      */
    uint64_t current = prime_cache[prime_cnt - 1] + 1;

    while (prime_cnt < (size_t)need) {
        /* add a new prime if candidate is itself prime */
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt && prime_cache[j] * prime_cache[j] <= current; ++j) {
            if (current % prime_cache[j] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* grow cache if needed */
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
                if (!tmp)
                    return;                       /* memory failure – abort */
                prime_cache = tmp;
            }
            prime_cache[prime_cnt++] = current;
        }
        ++current;
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                       /* guard – OEIS offset is 1 */

    /* make sure the n‑th prime is available */
    ensure_primes(n);
    uint64_t p = prime_cache[n - 1];

    /* exact floor(p / log p) using long double for safety */
    long double ld  = (long double)p;
    long double ldp = logl(ld);
    /* if p==2, ldp is log(2) > 0 */
    long double div = ld / ldp;
    /* rounding down – add a tiny epsilon to avoid FP rounding issues */
    unsigned long long f = (unsigned long long)floorl(div + 1e-12L);

    seq_t n_seq = (seq_t)n;
    seq_t f_seq = (seq_t)f;                   /* convert to seq_t safely */
    return n_seq - f_seq;
}
