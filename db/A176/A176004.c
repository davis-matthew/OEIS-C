/*
 * A176004  (n-th prime > 3) minus (n-th semiprime).
 * https://oeis.org/A176004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+2) – semiprime(n) (offset 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/* Internal state --------------------------------------------------------*/
/* --------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;  /* primes[0] = 2, primes[1] = 3, ... */
static size_t   prime_cnt     = 0;
static bool     primes_built  = false;

static uint64_t *semiprime_list = NULL; /* semiprime[0] = 4, semiprime[1] = 6, ... */
static size_t   semiprime_cnt   = 0;
static size_t   semiprime_cap   = 0;
static uint64_t next_semiprime  = 4;
static bool     semiprimes_built = false;

/* --------------------------------------------------------------------- */
/* Utility functions ----------------------------------------------------*/
/* --------------------------------------------------------------------- */

/* Simple sieve of Eratosthenes: builds all primes <= limit */
static void build_primes_upto_limit(idx_t limit)
{
    if (primes_built) return;

    size_t lim = (size_t)limit;
    bool *sieve = (bool*)malloc(lim + 1);
    for (size_t i = 0; i <= lim; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= lim; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= lim; j += i)
                sieve[j] = false;

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (sieve[i]) prime_list[idx++] = (uint64_t)i;

    prime_cnt = cnt;
    primes_built = true;
    free(sieve);
}

/* Count total prime factors with multiplicity: returns true iff exactly 2 */
static bool is_semiprime_uint64(uint64_t v)
{
    uint64_t t = v;
    int cnt = 0;
    for (uint64_t i = 2; i * i <= t && cnt <= 2; ++i)
    {
        while (t % i == 0)
        {
            t /= i;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (t > 1) ++cnt;
    return cnt == 2;
}

/* Grow the semiprime array to hold at least target elements */
static void build_semiprimes_up_to(size_t target)
{
    if (!semiprimes_built)
    {
        semiprime_cap = 64;
        semiprime_list = (uint64_t*)malloc(semiprime_cap * sizeof(uint64_t));
        semiprime_cnt = 0;
        semiprimes_built = true;
    }
    while (semiprime_cnt < target)
    {
        if (is_semiprime_uint64(next_semiprime))
        {
            if (semiprime_cnt == semiprime_cap)
            {
                semiprime_cap <<= 1;
                semiprime_list = (uint64_t*)realloc(semiprime_list,
                                                    semiprime_cap * sizeof(uint64_t));
            }
            semiprime_list[semiprime_cnt++] = next_semiprime;
        }
        ++next_semiprime;
    }
}

/* --------------------------------------------------------------------- */
/* Public function -------------------------------------------------------*/
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* not used in the OEIS sequence but safe guard */
        return (seq_t)0;

    /* ------- Ensure we have enough primes: prime(n+2) ---------- */
    size_t needed_primes = (size_t)(n + 2) + 1;   /* +1 because index starts at 0 */
    if (!primes_built || prime_cnt < needed_primes)
    {
        /* Rough upper bound for the (n+2)-th prime (Rosser's theorem) */
        double nd = (double)(n + 2);
        double approx = nd * (log(nd) + log(log(nd))) + 10.0;
        idx_t limit = (idx_t)(approx > 6000000 ? approx : 6000000);
        build_primes_upto_limit(limit);
    }

    /* ------- Ensure we have at least n semiprimes ---------------- */
    if (!semiprimes_built || semiprime_cnt < (size_t)n)
        build_semiprimes_up_to((size_t)n);

    /* ------- Retrieve the required values ------------------------ */
    seq_t prime_val   = (seq_t)prime_list[(size_t)(n + 1)];   /* prime(n+2) */
    seq_t semiprime_val = (seq_t)semiprime_list[(size_t)(n - 1)]; /* semiprime(n) */

    return prime_val - semiprime_val;
}
