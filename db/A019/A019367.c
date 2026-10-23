/*
 * A019367  Primes with primitive root 42.
 * https://oeis.org/A019367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019367: Primes with primitive root 42
   Formula: a(n) = the n‑th prime p (in increasing order) such that
   42 is a primitive root modulo p.  In other words,
   for every prime divisor q of p-1, 42^((p-1)/q) ≠ 1 (mod p).  */

/* Standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Holds the smallest prime factor for each number up to LIMIT. */
static uint32_t *spf;

/* List of all generated primes up to LIMIT. */
static idx_t *all_primes;
static size_t all_primes_cnt;

/* List of primes satisfying the primitive‑root condition. */
static idx_t *good_primes;
static size_t good_primes_cnt;

/* Flag to ensure initialization happens only once. */
static bool initialized = false;

/* Mod‑power for 64‑bit values using 128‑bit intermediate multiplication. */
static seq_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1U)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base   = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1U;
    }
    return result;
}

/* Test whether 42 is a primitive root modulo the prime p.  It assumes
   that spf is already filled for all numbers up to p. */
static bool is_primitive_root(idx_t p)
{
    if (p == 2 || p == 3 || p == 7)   /* 42 is not coprime to p. */
        return false;

    idx_t m = p - 1;
    idx_t tmp = m;
    idx_t base = 42 % p;

    while (tmp > 1) {
        uint32_t q = spf[tmp];            /* prime divisor of tmp */
        while (tmp % q == 0)
            tmp /= q;                      /* remove all power of q */

        if (mod_pow_uint64(base, m / q, p) == 1)
            return false;                  /* not a primitive root */
    }
    return true;
}

/* Initialise all static data: sieve, prime lists and the good primes. */
static void init_sequence(void)
{
    if (initialized) return;

    /* Choose a fixed upper bound that is large enough for n≈10000. */
    const idx_t LIMIT = 2000000U;   /* 2 000 000 */

    /* ---- Sieve of Eratosthenes with smallest prime factor array ---- */
    spf = (uint32_t *)malloc((LIMIT + 1U) * sizeof(uint32_t));
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0U;

    /* Allocate space for the primes (rough upper bound) */
    size_t primes_cap = LIMIT / 10U;
    all_primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    all_primes_cnt = 0U;

    for (idx_t i = 2U; i <= LIMIT; ++i) {
        if (spf[i] == 0U) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (all_primes_cnt >= primes_cap) {
                primes_cap *= 2U;
                all_primes = (idx_t *)realloc(all_primes, primes_cap * sizeof(idx_t));
            }
            all_primes[all_primes_cnt++] = i;

            /* Mark multiples with this smallest prime factor. */
            if ((uint64_t)i * (uint64_t)i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0U) spf[j] = (uint32_t)i;
            }
        }
    }

    /* ---- Collect primes that have 42 as a primitive root ---- */
    good_primes    = (idx_t *)malloc(all_primes_cnt * sizeof(idx_t));
    good_primes_cnt = 0U;

    for (size_t i = 0U; i < all_primes_cnt; ++i) {
        idx_t p = all_primes[i];
        if (is_primitive_root(p)) {
            good_primes[good_primes_cnt++] = p;
        }
    }

    /* We no longer need the list of all primes. */
    free(all_primes);
    free(spf);

    initialized = true;
}

/* ------------------------------------------------------------ */
/* Public API required by the framework.                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0U)
        return 0U;       /* undefined for n = 0, return 0 for safety */

    init_sequence();

    if (n <= good_primes_cnt)
        return (seq_t)good_primes[n - 1U];
    return 0U;           /* if n is out of range */
}
