/*
 * A077008  Legendre symbol (-1,p) where p is the n-th prime.
 * https://oeis.org/A077008
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Legendre symbol (−1|prime(n)) * = 1 if prime(n) ≡ 1 (mod
 *            4) * = –1 if prime(n) ≡ 3 (mod 4) * (offset 2, so a(2) uses the
 *            second prime 3)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Static storage for primes */
static uint8_t   *is_composite = NULL;   /* 1‑byte flag per integer */
static uint64_t  *prime_arr    = NULL;   /* list of primes */
static size_t    prime_cnt    = 0;        /* number of primes stored */
static size_t    prime_cap    = 0;        /* current capacity of prime_arr */
static size_t    current_limit = 0;       /* current sieve limit */

/* ------------------------------------------------------------
 * Estimate an upper bound for the n‑th prime.
 * Uses the Rosser–Schoenfeld bound:  p_n < n (ln n + ln ln n)  for n ≥ 6
 * ------------------------------------------------------------ */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6)
        return 15;                     /* covers tiny n */
    double nn = (double)n;
    double logn = log(nn);
    double loglogn = log(logn);
    double estimate = nn * (logn + loglogn);
    return (idx_t)(estimate + 3.0);    /* safety margin */
}

/* ------------------------------------------------------------
 * Fast Sieve of Eratosthenes up to a given bound.
 * Stores all primes in prime_arr; rebuilds the sieve if limit increases.
 * ------------------------------------------------------------ */
static void update_sieve(idx_t n_needed)
{
    idx_t limit = estimate_limit(n_needed);
    if (limit <= current_limit)           /* already enough primes */
        return;

    /* release old structures */
    free(is_composite);
    free(prime_arr);

    is_composite = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_composite)
        return;                           /* out of memory – unlikely */
    memset(is_composite, 0, limit + 1);

    prime_arr = NULL;
    prime_cnt = 0;
    prime_cap = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            /* grow prime array if needed */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap == 0 ? 64 : prime_cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_arr,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp)
                    return;               /* out of memory */
                prime_arr = tmp;
                prime_cap = new_cap;
            }
            prime_arr[prime_cnt++] = i;

            if (i <= limit / i) {        /* avoid overflow */
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    current_limit = limit;
}

/* Ensure that at least n primes are available */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;
    update_sieve(n);
}

/* ------------------------------------------------------------
 * Sequence: Legendre symbol (−1|p_n)  (offset 2)
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                               /* offset 2; behaviour below irrelevant */

    ensure_primes(n);

    idx_t p = prime_arr[n - 1];                  /* primes are 1‑based in the sequence */
    seq_t result = (p % 4 == 1) ? (seq_t)1 : (seq_t)-1;
    return result;
}
