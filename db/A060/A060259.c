/*
 * A060259  Denoting 4 consecutive primes by p, q, r and s, these are the
 *          values of q such that q and r have 10 as a primitive root, but p
 *          and s do not.
 * https://oeis.org/A060259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 384, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A060259 (OEIS) – the q‑values of prime quadruples
 * p,q,r,s where 10 is a primitive root modulo q and r
 * but not modulo p or s.
 * The routine sieves primes up to a fixed limit,
 * tests each for the primitive‑root property by
 * modular exponentiation, and collects the required q’s
 * into a cache.  a(n) returns the n‑th cached value. */
 
#include <stdint.h>
#include <stdlib.h>

/* ---------- basic types (supplied by the build) ---------- */
extern seq_t a(idx_t n);  /* declaration for the build */

/* ---------- static data ---------------------------------- */
#define LIMIT      5000000u          /* prime sieve bound (≈5 M) */
#define PRIMES_CAP 600000u          /* upper bound on number of primes */

static uint32_t *g_primes      = NULL; /* list of primes */
static size_t   g_prime_count  = 0;    /* number of primes */

static uint8_t  *g_isPR10      = NULL; /* 1 if prime has 10 as a primitive root */
static seq_t    *g_cache       = NULL; /* stored q‑values of the sequence */
static size_t   g_cache_size   = 0;    /* number of terms cached */

static int      g_inited       = 0;     /* flag */

/* ---------- utility functions ---------------------------- */
/* modular exponentiation (binary exponentiation, 64‑bit safe) */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1u) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1u;
    }
    return res;
}

/* test whether 10 is a primitive root modulo prime p */
static uint8_t has10_primitive_root(uint32_t p)
{
    /* 10 cannot be primitive root modulo 2 or 5 */
    if (p == 2u || p == 5u) return 0;

    /* factorise p-1 using the prime list */
    uint32_t phi = p - 1u;
    uint32_t temp = phi;
    for (size_t i = 0; i < g_prime_count && (uint64_t)g_primes[i] * g_primes[i] <= temp; ++i) {
        uint32_t q = g_primes[i];
        if (temp % q == 0u) {
            /* 10^((p-1)/q) mod p must NOT be 1 */
            if (mod_pow_uint64(10u, (uint64_t)phi / q, (uint64_t)p) == 1u)
                return 0;
            while (temp % q == 0u) temp /= q;
        }
    }
    if (temp > 1u) {          /* remaining prime factor */
        if (mod_pow_uint64(10u, (uint64_t)phi / temp, (uint64_t)p) == 1u)
            return 0;
    }
    return 1;
}

/* ---------- initialization -------------------------------- */
static void init(void)
{
    if (g_inited) return;
    g_inited = 1;

    /* allocate arrays */
    g_primes = (uint32_t *)malloc(PRIMES_CAP * sizeof(uint32_t));
    g_isPR10 = (uint8_t  *)malloc(PRIMES_CAP * sizeof(uint8_t));
    g_cache  = (seq_t   *)malloc(PRIMES_CAP * sizeof(seq_t));

    /* mark composites with a simple sieve (bytes per number) */
    uint8_t *is_comp = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    for (uint32_t i = 2u; i <= LIMIT; ++i) is_comp[i] = 0;

    /* first pass: generate prime list */
    for (uint32_t i = 2u; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (g_prime_count < PRIMES_CAP) g_primes[g_prime_count++] = i;
            if ((uint64_t)i * i <= (uint64_t)LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    /* compute 10‑primitive‑root flag for each prime */
    for (size_t i = 0; i < g_prime_count; ++i)
        g_isPR10[i] = has10_primitive_root(g_primes[i]);

    /* scan consecutive quadruples and cache the required q’s */
    for (size_t i = 0; i + 3U < g_prime_count; ++i) {
        if (!g_isPR10[i] && g_isPR10[i + 1U] && g_isPR10[i + 2U] && !g_isPR10[i + 3U]) {
            if (g_cache_size < PRIMES_CAP)
                g_cache[g_cache_size++] = (seq_t)g_primes[i + 1U];
        }
    }

    /* free temporary array */
    free(is_comp);
}

/* ---------- required function ----------------------------- */
extern seq_t a(idx_t n)   /* n ≥ 1 */
{
    init();                            /* ensure data is ready */
    if ((idx_t)g_cache_size < n) return (seq_t)0; /* out of range – undefined */
    return g_cache[(size_t)(n - 1)];
}
