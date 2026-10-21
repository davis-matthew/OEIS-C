/*
 * A216845  Numbers n such that the polynomial 1 + x + x^2 + x^3 + x^4 + ... +
 *          x^(n-1) is reducible over GF(2).
 * https://oeis.org/A216845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216845: Numbers n such that 1 + x + x² + … + x^(n-1) is reducible over GF(2)
   (i.e. all composite n and primes for which 2 is not a primitive root).
   The function searches consecutive integers, keeping a cache,
   and returns the nth term (offset 1). */

/*==========================================================================*/
/*  Standard headers – needed for bool, malloc, size_t, uint64_t            */
/*==========================================================================*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*==========================================================================*/
/*  Prime sieve data (one-time global initialisation)                       */
/*==========================================================================*/
static const size_t PRIME_SIEVE_LIMIT = 2000000U;         /* upper bound for pre‑computed primes */
static bool *prime_is_ok   = NULL;                       /* sieve table */
static uint32_t *prime_list = NULL;                      /* list of primes up to the limit */
static size_t prime_cnt   = 0;                            /* number of primes in prime_list */

/*==========================================================================*/
/*  Initialise the prime sieve once                                                */
/*==========================================================================*/
static void init_prime_sieve(void)
{
    if (prime_is_ok != NULL) return;         /* already initialised */

    prime_is_ok = (bool*)calloc(PRIME_SIEVE_LIMIT + 1U, sizeof(bool));
    if (!prime_is_ok) exit(1);
    for (size_t i = 2U; i <= PRIME_SIEVE_LIMIT; ++i) prime_is_ok[i] = true;

    for (size_t i = 2U; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (prime_is_ok[i])
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                prime_is_ok[j] = false;

    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2U; i <= PRIME_SIEVE_LIMIT; ++i)
        if (prime_is_ok[i]) ++prime_cnt;

    /* Build prime list */
    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (size_t i = 2U; i <= PRIME_SIEVE_LIMIT; ++i)
        if (prime_is_ok[i]) prime_list[idx++] = (uint32_t)i;
}

/*==========================================================================*/
/*  Utility: modular exponentiation (uint64_t version)                        */
/*==========================================================================*/
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1ULL) return 0ULL;
    uint64_t result = 1ULL % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1ULL;
    }
    return result;
}

/*==========================================================================*/
/*  Test whether n is prime (uint64_t) using the pre‑computed sieve          */
/*==========================================================================*/
static bool is_prime_uint64(uint64_t n)
{
    if (n <= 1ULL) return false;
    if (n <= PRIME_SIEVE_LIMIT) return prime_is_ok[(size_t)n];

    for (size_t i = 0ULL; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p;
        if (pp * pp > n) break;
        if (n % pp == 0ULL) return false;
    }
    return true;
}

/*==========================================================================*/
/*  Check whether 2 is a primitive root modulo the prime p                  */
/*==========================================================================*/
static bool is_primitive_root_prime(uint64_t p)
{
    if (p <= 3ULL) return true;          /* 2 is a primitive root modulo 3, and 2 itself is irrelevant */

    uint64_t phi   = p - 1ULL;
    uint64_t trial = phi;

    for (size_t i = 0ULL; i < prime_cnt; ++i) {
        uint32_t q = prime_list[i];
        uint64_t qq = (uint64_t)q;
        if (qq * qq > trial) break;
        if (trial % qq == 0ULL) {
            if (mod_pow_uint64(2ULL, phi / qq, p) == 1ULL) return false;
            while (trial % qq == 0ULL) trial /= qq;
        }
    }

    if (trial > 1ULL) {                     /* remaining factor */
        if (mod_pow_uint64(2ULL, phi / trial, p) == 1ULL) return false;
    }

    return true;
}

/*==========================================================================*/
/*  Cache for sequence values                                               */
/*==========================================================================*/
static seq_t *seq_cache  = NULL;      /* array of earlier terms          */
static size_t seq_len   = 0;          /* current number of stored terms   */
static size_t seq_cap   = 0;          /* current capacity of seq_cache   */
static uint64_t last_checked = 3ULL;   /* next number to test (begins at 4) */

/*==========================================================================*/
/*  Ensure seq_cache has capacity for at least 'need' elements              */
/*==========================================================================*/
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2ULL : 1ULL;
    while (new_cap < need) new_cap *= 2ULL;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache) exit(1);
    seq_cap = new_cap;
}

/*==========================================================================*/
/*  Main function: compute a(n)                                               */
/*==========================================================================*/
seq_t a(idx_t n)
{
    init_prime_sieve();                /* guarantee primes are ready */

    if (n == 0) return 0;              /* safety return for non‑positive indices */

    /* Generate terms until we have at least 'n' of them */
    while (seq_len < (size_t)n) {
        ++last_checked;
        uint64_t candidate = last_checked;
        bool reducible;

        if (candidate <= 3ULL) {
            reducible = false;         /* 1, 2, 3 are not reducible */
        } else if (!is_prime_uint64(candidate)) {
            reducible = true;          /* all composite numbers are reducible */
        } else {
            /* prime: reducible iff 2 is NOT a primitive root */
            reducible = !is_primitive_root_prime(candidate);
        }

        if (reducible) {
            ensure_seq_capacity(seq_len + 1ULL);
            seq_cache[seq_len++] = (seq_t)candidate;
        }
    }

    return seq_cache[(size_t)n - 1ULL];
}
