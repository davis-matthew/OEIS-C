/*
 * A175248  Noncomposites (A008578) with noncomposite (A008578) subscripts.
 * https://oeis.org/A175248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008578(A008578(n)) (noncomposite numbers of order 2)
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Prime cache (1‑based indices) */
static seq_t *primes   = NULL;   /* array of primes              */
static size_t prime_cnt;         /* number of primes in array    */
static idx_t  prime_limit;       /* highest value covered by array */

/* --------------------------------------------------------------- */
/* Estimate an upper bound for the n‑th prime (n >= 1).            */
/* Uses p_n < n (log n + log log n) for n >= 6.                  */
static idx_t
estimate_prime_upper_bound(idx_t n)
{
    if (n < 6) return (idx_t)15;               /* p_5 = 11 < 15          */
    double dn   = (double)n;
    double bound = dn * (log(dn) + log(log(dn)));
    return (idx_t)(bound + 3.0);              /* round up a little       */
}

/* --------------------------------------------------------------- */
/* Sieve of Eratosthenes up to 'limit' (inclusive).               */
/* Builds a fresh prime list in the static cache.                 */
static void
generate_primes_up_to(idx_t limit)
{
    /* Allocate bitmap: 0 = prime, 1 = composite.               */
    uint8_t *is_cmp = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_cmp) return;                        /* ignore failure       */

    is_cmp[0] = is_cmp[1] = 1;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_cmp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_cmp[j] = 1;
        }
    }

    /* Count primes to get array size. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_cmp[i]) ++cnt;

    /* Allocate array of primes. */
    seq_t *new_pr = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!new_pr) { free(is_cmp); return; }

    /* Copy primes into array. */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_cmp[i]) new_pr[pos++] = (seq_t)i;

    free(is_cmp);

    /* Replace old cache. */
    if (primes) free(primes);
    primes     = new_pr;
    prime_cnt  = cnt;
    prime_limit = limit;
}

/* --------------------------------------------------------------- */
/* Ensure that at least 'n' primes are present in the cache.      */
static void
ensure_primes_for_idx(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    idx_t lim = estimate_prime_upper_bound(n);
    generate_primes_up_to(lim);

    /* In case the estimate was too low, keep doubling.          */
    while (prime_cnt < (size_t)n) {
        if (lim < prime_limit) lim = prime_limit + 1;
        else lim *= 2;
        generate_primes_up_to(lim);
    }
}

/* --------------------------------------------------------------- */
/* Return the n‑th prime (1‑based).  Returns 0 for n == 0.         */
static seq_t
prime_at(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* Caller guarantees n <= prime_cnt. */
    return primes[n - 1];
}

/* --------------------------------------------------------------- */
/* Main sequence function.                                         */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;            /* a(1) = 1 (a(0) undefined) */

    /* First non‑composite: q(n) = prime(n-1).                    */
    ensure_primes_for_idx(n - 1);
    seq_t k = prime_at(n - 1);

    /* Second non‑composite: q(k) = prime(k-1).                    */
    idx_t idx = (idx_t)(k - 1);             /* k ≥ 2 for n ≥ 2            */
    ensure_primes_for_idx(idx);
    return prime_at(idx);
}
