/*
 * A175073  Primes q with result 1 under iterations of {r mod (max prime p <
 *          r)} starting at r = q.
 * https://oeis.org/A175073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime q (q ≠ 2) such that the iterative process r ← r
 *            mod p (where p is the largest prime less than r) starting at r =
 *            q eventually reaches 1. The algorithm builds a sieve up to a
 *            fixed limit, precomputes for each number its preceding prime,
 *            then tests each prime in order until the req...
 * Range:     u8 n <= 36, u16 n <= 5588, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment */
static idx_t *prev_prime = NULL;          /* largest prime < i for each i */
static uint32_t *primes = NULL;           /* list of primes ≤ LIMIT */
static size_t prime_count = 0;            /* number of primes found */

static seq_t *seq_cache = NULL;           /* sequence terms */
static idx_t seq_count = 0;               /* number of terms stored so far */
static size_t seq_cap = 0;                /* current allocation size */
static idx_t next_prime_idx = 0;          /* next prime index to test */

/* fixed upper bound: enough to contain the first 10000 terms */
#define BUILD_LIMIT (2000000)

/* build sieve, prev_prime table, and prime list */
static void build_sieve(void)
{
    if (prev_prime != NULL)
        return; /* already built */

    const idx_t limit = BUILD_LIMIT;
    /* Allocate arrays */
    prev_prime = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!prev_prime || !is_prime) {
        /* In a build-error scenario */
        exit(1);
    }

    /* Initialize prime flags */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Build prev_prime array (largest prime < i) */
    idx_t last = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prev_prime[i] = last;
            last = i;
        } else {
            prev_prime[i] = last;
        }
    }

    /* Count primes */
    size_t estim = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++estim;

    /* Allocate primes array */
    primes = (uint32_t *)malloc(estim * sizeof(uint32_t));
    if (!primes) {
        free(prev_prime);
        free(is_prime);
        exit(1);
    }

    /* Fill primes array */
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;

    free(is_prime);
}

/* test whether a prime q satisfies the iteration criterion */
static bool passes(uint32_t q)
{
    idx_t r = q;
    while (r > 1) {
        idx_t p = prev_prime[r];
        if (p == 0)          /* r is 2 or no smaller prime */
            return false;
        r = r % p;
        if (r == 0)          /* would enter 0 => impossible to reach 1 */
            return false;
    }
    return true;
}

/* fill seq_cache until it contains at least n terms */
static void ensure_sequence(idx_t n)
{
    build_sieve();           /* guarantee sieve and primes ready */

    if (seq_count >= n)
        return;              /* already enough */

    /* Grow seq_cache as needed */
    if (seq_cap == 0) {
        seq_cap = 128;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }

    /* Test primes and collect qualifying ones */
    for (; next_prime_idx < prime_count && seq_count < n; ++next_prime_idx) {
        uint32_t q = primes[next_prime_idx];
        if (q == 2)      /* skip 2: it never reaches 1 */
            continue;
        if (passes(q)) {
            if (seq_count == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!seq_cache) exit(1);
            }
            seq_cache[seq_count++] = (seq_t)q;
        }
    }
    /* If we reach this point and still lack enough terms, the limit
       was insufficient, but the problem guarantees at most 10000 terms. */
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_sequence(n);
    return seq_cache[n - 1];   /* offset 1 */
}
