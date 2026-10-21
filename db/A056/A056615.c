/*
 * A056615  Binomial(2*n - 1, n - 1) - 1 (mod n^2).
 * https://oeis.org/A056615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(2*n - 1, n - 1) - 1 (mod n^2)
 * Range:     u8 n <= 15, u16 n <= 271, u32 n <= 1811, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modpow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/modpow_seq_t.h"

/* Prime sieve cache ---------------------------------------------------------------- */

static int   *prime_list = NULL;   /* array of primes */
static size_t prime_cnt  = 0;      /* number of primes stored */
static idx_t  sieve_max  = 0;      /* current maximal n with primes up to 2*n - 1 */

/* Ensure prime_list contains all primes ≤ 2*n - 1 */
static void ensure_primes(idx_t n)
{
    idx_t limit = 2 * n + 1;        /* we sieve up to 2*n (inclusive) */
    if (sieve_max >= limit)
        return;

    /* Allocate and initialise sieve */
    size_t sz = (size_t)(limit + 1);
    unsigned char *sieve = (unsigned char *)malloc(sz);
    if (!sieve) abort();
    memset(sieve, 1, sz);
    if (sz > 0) sieve[0] = 0;
    if (sz > 1) sieve[1] = 0;

    /* Eratosthenes */
    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++cnt;

    /* Allocate or reallocate prime list */
    int *new_list = (int *)realloc(prime_list, cnt * sizeof(int));
    if (!new_list) abort();

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            new_list[k++] = (int)i;

    free(sieve);
    prime_list = new_list;
    prime_cnt  = cnt;
    sieve_max  = limit;
}

/* Main routine --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Handle n = 0 defensively – the sequence is defined for n ≥ 1 */
    if (n == 0)
        return (seq_t)0;

    /* Modulus n^2 (fits into a seq_t for all n used in the tests) */
    seq_t ns = (seq_t)n;
    seq_t mod = ns * ns;            /* n * n */

    /* Initialise the sieve to contain all primes up to 2*n - 1 */
    ensure_primes(n);

    seq_t binom = 1 % mod;          /* stays 1 modulo `mod` */

    /* For each prime ≤ 2*n - 1 compute its exponent in (2*n-1 choose n-1) */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        int p = prime_list[idx];
        if ((idx_t)p > 2 * n - 1)
            break;

        /* Legendre exponents: vp((2n-1)!) - vp((n-1)!) - vp(n!) */
        uint64_t exp = 0;
        unsigned __int128 power = p;
        unsigned __int128 limit = (unsigned __int128)(2 * n - 1);

        while (power <= limit) {
            exp += (uint64_t)(limit / power
                        - (unsigned __int128)(n - 1) / power
                        - (unsigned __int128)n / power);
            power *= p;
        }
        if (exp == 0)
            continue;

        seq_t base = (seq_t)p % mod;
        seq_t part = modpow_seq_t(base, exp, mod);
        binom = (binom * part) % mod;
    }

    /* Result: binom - 1 (mod n^2) */
    seq_t ans = (binom + mod - 1) % mod;
    return ans;
}
