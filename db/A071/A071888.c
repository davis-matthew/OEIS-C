/*
 * A071888  a(1)=2 and for n>1 a(n) = least squarefree m > n with gcd(m,n)>1.
 * https://oeis.org/A071888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1)=2; for n>1 a(n) = least squarefree m>n with gcd(m,n)>1
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
   Prime table (sieve up to a fixed limit, reused by all calls)
   ---------------------------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 500000U                      /* enough for n up to ~1e5 */

static unsigned *prime_tbl = NULL;                    /* array of primes    */
static uint32_t prime_cnt = 0;                        /* number of primes   */

static void init_primes(void)
{
    if (prime_tbl != NULL) return;                    /* already done       */

    bool *sieve = (bool*)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!sieve) return;                              /* allocation failure */

    /* Mark all as prime initially */
    for (uint32_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    /* Classic Sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) if (sieve[i]) ++prime_cnt;

    /* Store primes in an array */
    prime_tbl = (unsigned*)malloc(prime_cnt * sizeof(unsigned));
    if (!prime_tbl) {
        free(sieve);
        return;
    }
    uint32_t pos = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) prime_tbl[pos++] = (unsigned)i;

    free(sieve);
}

/* ----------------------------------------------------------------------
   Auxiliary helpers
   ---------------------------------------------------------------------- */

/* Euclidean gcd for unsigned long long values */
static inline uint64_t gcd_ull(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Test whether a 64‑bit integer is square‑free */
static bool is_squarefree_uint64(uint64_t x)
{
    if (x <= 1) return true;
    init_primes();                         /* ensure prime list is ready */

    for (uint32_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        uint64_t p2 = p * p;
        if (p2 > x) break;
        if (x % p2 == 0) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------
   Main routine to compute a(n)
   ---------------------------------------------------------------------- */
static seq_t find_value(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    for (uint64_t m = n64 + 1; ; ++m) {
        if (gcd_ull(m, n64) > 1 && is_squarefree_uint64(m))
            return (seq_t)m;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    return find_value(n);
}
