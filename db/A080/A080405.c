/*
 * A080405  Number of distinct primes dividing n-th Catalan number.
 * https://oeis.org/A080405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of the n‑th Catalan
 *            number C(n)
 * Range:     u8 n <= 1286, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime handling ---------- */
static bool *sieve = NULL;
static idx_t sieve_limit = 0;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Build all primes up to 'limit' (inclusive) using a simple sieve.  The
   *sieve* array and the *primes* table are kept in static storage so they
   *are reused between successive calls to a().  If *limit* is smaller
   *than the current limit, nothing is done. */
static void build_primes_up_to(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate a fresh sieve and fill it. */
    bool *new_sieve = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!new_sieve)
        return;                 /* memory failure – leave function inert */

    for (idx_t i = 0; i <= limit; ++i)
        new_sieve[i] = true;
    new_sieve[0] = new_sieve[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (new_sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = false;

    /* Ensure the primes table is large enough. */
    size_t needs = (limit / 2) + 10;   /* crude upper bound for #primes */
    if (needs > prime_cap) {
        uint64_t *new_primes = (uint64_t*)realloc(primes, needs * sizeof(uint64_t));
        if (!new_primes) {   /* allocation failure – leak previous data, abort */
            free(new_sieve);
            return;
        }
        primes = new_primes;
        prime_cap = needs;
    }

    /* Re‑populate the primes array. */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_sieve[i])
            primes[prime_cnt++] = (uint64_t)i;

    /* Replace the old sieve. */
    free(sieve);
    sieve = new_sieve;
    sieve_limit = limit;
}

/* ---------- Legendre exponent of prime p in m! ---------- */
static inline idx_t legendre(idx_t m, uint64_t p)
{
    idx_t exp = 0;
    while (m)
    {
        m /= p;
        exp += m;
    }
    return exp;
}

/* ---------- Public routine ---------- */
seq_t a(idx_t n)
{
    idx_t two_n = 2 * n;
    build_primes_up_to(two_n);

    int count = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p > two_n)
            break;

        /* exponent of p in binom(2n,n) */
        idx_t exp_binom = legendre(two_n, p) - 2 * legendre(n, p);

        /* exponent of p in (n+1) */
        idx_t exp_n1 = 0;
        idx_t tmp = n + 1;
        while (tmp % p == 0)
        {
            ++exp_n1;
            tmp /= p;
        }

        if (exp_binom > exp_n1)
            ++count;
    }

    return (seq_t)count;
}
