/*
 * A225581  a(n) is the minimal odd prime q such that prime(n)*q + prime(n) +
 *          q is prime.
 * https://oeis.org/A225581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least odd prime q such that prime(n)*q + prime(n) + q
 *            is prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation pre‑computes primes up to a fixed limit using a simple
   sieve and then searches for the required q by testing odd primes from the
   sieve.  The search stops as soon as a q that satisfies the condition is
   found.  The function works for any n ≥ 1, independent of the order of
   calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* ------------------------------------------------------------ */
/*  Miller–Rabin for 64‑bit integers                             */
/* ------------------------------------------------------------ */

static inline uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/*  Sieve to obtain primes up to a fixed limit                 */
/* ------------------------------------------------------------ */
#define SIEVE_LIMIT 2000000   /* enough for first 10,000 primes and plenty of q's */

static char *sieve_tbl = NULL;          /* 0 = prime, 1 = composite */
static uint32_t *prime_tbl = NULL;      /* array of primes */
static size_t prime_cnt = 0;             /* number of primes stored */

static void init_sieve(void)
{
    if (sieve_tbl) return;               /* already initialised */

    size_t limit = SIEVE_LIMIT;
    sieve_tbl = (char *)calloc(limit + 1, sizeof(char));
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve_tbl[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_tbl[j] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve_tbl[i]) ++cnt;

    prime_tbl = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve_tbl[i]) prime_tbl[idx++] = (uint32_t)i;

    prime_cnt = cnt;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset is 1 */

    init_sieve();

    /* Safety: the sieve limit guarantees enough primes for n up to ~150k. */
    if ((idx_t)n > prime_cnt) {
        exit(1);
    }

    uint64_t p = (uint64_t)prime_tbl[n - 1];   /* nth prime (1‑based) */

    /* Search for the minimal odd prime q.  Start from the second prime (3). */
    for (size_t i = 1; ; ++i) {
        uint64_t q = (uint64_t)prime_tbl[i];
        uint64_t m = p * q + p + q;
        if (isPrime64(m))
            return (seq_t)q;
    }
}
