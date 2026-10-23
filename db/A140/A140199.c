/*
 * A140199  a(n) = the number of pairs of (not necessarily distinct) positive
 *          integers j and k where j <= n and k <= n such that k+j is prime.
 * https://oeis.org/A140199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (π(2k) – π(k)) where π(x) is the number of
 *            primes ≤ x. This equals the number of pairs (j,k) with 1 ≤ j,k ≤
 *            n and j + k prime
 * Range:     u8 n <= 45, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- static state for sieving and prefix sums ---------- */
static unsigned char *is_prime = NULL;   /* 1 = prime, 0 = composite */
static uint64_t   *prime_pi  = NULL;     /* π(x) for all x ≤ prime_limit */
static idx_t      prime_limit = 0;       /* largest value sieved so far */

static seq_t *cum_arr = NULL;           /* cumulative a(k) + a(0) = 0 */
static idx_t cum_size = 0;              /* largest n cached */

/* ---------- ensure primes up to `limit` (inclusive) ---------- */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free previous tables if any */
    free(is_prime);
    free(prime_pi);

    /* allocate new tables */
    size_t sz = (size_t)(limit + 1);           /* need index 0..limit */
    is_prime = (unsigned char *)malloc(sz);
    prime_pi = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!is_prime || !prime_pi) {
        /* memory allocation failed – abort program */
        abort();
    }

    /* initialise all entries as prime (will clear 0,1 next) */
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[(size_t)p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[(size_t)j] = 0;
        }
    }

    /* build prefix counts π(x) */
    uint64_t cnt = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[(size_t)i])
            ++cnt;
        prime_pi[(size_t)i] = cnt;
    }

    prime_limit = limit;
}

/* ---------- ensure cumulative array up to n ---------- */
static void ensure_cum(idx_t n)
{
    if (n <= cum_size)
        return;

    /* primes up to 2n are required to evaluate π(2k) for k ≤ n */
    ensure_primes(2 * n);

    /* initialise cumulative array if first time */
    if (!cum_arr) {
        cum_arr = (seq_t *)malloc(sizeof(seq_t));   /* size 1 */
        if (!cum_arr) abort();
        cum_arr[0] = (seq_t)0;
        cum_size = 0;
    }

    /* grow cumulative array to size n+1 */
    seq_t *tmp = (seq_t *)realloc(cum_arr, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cum_arr = tmp;
    /* initialise new slots to zero – they will be overwritten below */
    for (idx_t i = cum_size + 1; i <= n; ++i)
        cum_arr[i] = (seq_t)0;

    /* compute missing cumulative values */
    for (idx_t k = cum_size + 1; k <= n; ++k) {
        seq_t addend = (seq_t)(prime_pi[(size_t)(2 * k)] - prime_pi[(size_t)k]);
        cum_arr[k] = cum_arr[k - 1] + addend;
    }

    cum_size = n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;       /* undefined for n < 1 – return 0 */

    ensure_cum(n);
    return cum_arr[n];
}
