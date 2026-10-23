/*
 * A154728  Products of three consecutive primes of the form 6n+1 (see
 *          A002476).
 * https://oeis.org/A154728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p_n * p_{n+1} * p_{n+2} where p_i is the i‑th prime ≡ 1
 *            (mod 6). Indexing starts at 1
 * Range:     u8 none, u16 n <= 4, u32 n <= 122, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Internal storage of primes ≡ 1 mod 6                               */
/* ------------------------------------------------------------------ */
static idx_t        *primes      = NULL;      /* array of primes ≡1 mod 6 */
static size_t       primes_len   = 0;         /* number of primes stored */

/* ------------------------------------------------------------------ */
/*  Sieve of Eratosthenes up to `limit`, collecting primes ≡ 1 mod 6  */
/* ------------------------------------------------------------------ */
static void recompute_primes(idx_t limit)
{
    /* sieve array */
    size_t sz = (size_t)(limit + 1);
    char *is_prime = (char *)malloc(sz);
    if (!is_prime) exit(1);
    memset(is_prime, 1, sz);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* free previous list */
    if (primes) free(primes);
    primes = NULL;
    primes_len = 0;

    /* allocate temporary buffer with rough upper bound */
    size_t capacity = (size_t)(limit / 8 + 1);
    primes = (idx_t *)malloc(capacity * sizeof(idx_t));
    if (!primes) exit(1);

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i] && (i % 6 == 1)) {
            if (primes_len == capacity) {
                capacity <<= 1;
                primes = (idx_t *)realloc(primes, capacity * sizeof(idx_t));
                if (!primes) exit(1);
            }
            primes[primes_len++] = i;
        }
    }

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/*  Ensure that at least `need` primes of the form 6k+1 are available  */
/* ------------------------------------------------------------------ */
static void ensure_primes(idx_t need)
{
    static idx_t  current_limit  = 400000;   /* initial sieve bound   */
    static bool   initialized   = false;     /* has sieve been built? */

    if (!initialized) {
        recompute_primes(current_limit);
        initialized = true;
    }
    while (primes_len < need) {
        current_limit <<= 1;                 /* double the bound       */
        recompute_primes(current_limit);
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence function: returns a(n)                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for offset 0 */

    /* we need n+2 primes to form the product for a(n) */
    ensure_primes(n + 2);

    idx_t idx = n - 1;                       /* 0‑based index in primes array */

    seq_t p0 = primes[idx];
    seq_t p1 = primes[idx + 1];
    seq_t p2 = primes[idx + 2];

    /* compute product; use built‑in integer arithmetic which will
       truncate correctly when the result exceeds seq_t's width    */
    return (p0 * p1) * p2;
}
