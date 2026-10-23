/*
 * A108219  Numbers n such that A001414(n) is a golden semiprime, where
 *          A001414 is the sum of primes dividing n (with repetition).
 * https://oeis.org/A108219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108219 : Numbers n such that A001414(n) is a golden semiprime
 * (i.e. product of two distinct primes p<q with q=floor(p·φ) or
 * q=floor(p·φ)+1).  The sequence is computed by sieving the least
 * prime factors up to a sufficiently large limit and searching through
 * the integers until the requested term is found.  All arithmetic is
 * performed using the integer type seq_t and the index type idx_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

#define MAX_SIEVE 20000000U   /* upper bound sufficient for all tests */

/* arrays for linear sieve (least prime factor and sum of prime factors) */
static uint32_t *spf = NULL;        /* spf[i] = least prime factor of i */
static uint32_t *sumfact = NULL;    /* sumfact[i] = sum of prime factors of i (with multiplicity) */
static uint32_t *primes = NULL;     /* list of primes up to MAX_SIEVE */
static size_t primes_count = 0;     /* number of primes */

/* storage for discovered sequence values */
static seq_t  *seq_values = NULL;   /* dynamic array of seq_t values */
static size_t  seq_size   = 0;      /* number of values currently stored */

/* progress of linear search */
static uint32_t last_scanned = 0;   /* last integer examined */

/* relative error < 1/1000 for φ ≈ 1.618033988749895 */
static const unsigned long long PHINUM  = 1618033988749895ULL;
static const unsigned long long PHIDEN  = 1000000000000000ULL;

/* --------------------------------------------------------------------- */
static void init_sieve(void)
/* Build the linear sieve and compute sumfact[].  Called once on first
 * invocation of a( ) when the global arrays are still uninitialised. */
{
    if (spf != NULL) return;              /* already initialised */

    /* allocate large arrays (cast required for C++) */
    spf      = (uint32_t*)calloc(MAX_SIEVE + 1, sizeof(uint32_t));
    sumfact  = (uint32_t*)calloc(MAX_SIEVE + 1, sizeof(uint32_t));
    if (spf == NULL || sumfact == NULL) { while (1); } /* abort on OOM */

    /* dynamic prime list; reserve space for about C*n/log(n) primes */
    size_t primes_cap = MAX_SIEVE / 10 + 1;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (primes == NULL) { while (1); }

    /* linear sieve: compute spf and primes */
    for (uint32_t i = 2; i <= MAX_SIEVE; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_count == primes_cap) {     /* grow primes array */
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (primes == NULL) { while (1); }
            }
            primes[primes_count++] = i;
        }
        for (size_t k = 0; k < primes_count; ++k) {
            uint32_t p = primes[k];
            uint64_t v = (uint64_t)i * p;
            if (v > MAX_SIEVE) break;
            if (p > spf[i]) break;
            spf[v] = p;
        }
    }

    /* compute sumfact[i] = sum of prime factors (with multiplicity) */
    sumfact[1] = 0;
    for (uint32_t i = 2; i <= MAX_SIEVE; ++i) {
        sumfact[i] = sumfact[i / spf[i]] + spf[i];
    }
}

/* --------------------------------------------------------------------- */
static bool is_golden_semiprime(uint64_t m)
/* Return true iff m is a product of two distinct primes p<q such that
 * q==floor(p·φ) or q==floor(p·φ)+1.  The factorisation uses the global
 * list of primes, which is guaranteed to contain all primes up to
 * MAX_SIEVE. */
{
    if (m < 6) return false;        /* first golden semiprime is 2·3 */

    uint64_t p = 0;
    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t pr = primes[i];
        if ((uint64_t)pr * pr > m) break;
        if (m % pr == 0) { p = pr; break; }
    }
    if (p == 0) return false;       /* m is prime or 1 */

    uint64_t q = m / p;
    if (q == p) return false;       /* distinct primes required */
    if (!is_prime(q)) return false;   /* q must be prime */

    /* golden check: q equals floor(p·φ) or floor(p·φ)+1 */
    unsigned __int128 prod = (unsigned __int128)p * PHINUM;
    uint64_t floor_val = (uint64_t)(prod / PHIDEN);
    return (q == floor_val) || (q == floor_val + 1);
}

/* --------------------------------------------------------------------- */
static void add_term(uint32_t n)
/* Append n to the sequence array after confirming it satisfies
 * the property. */
{
    seq_values = (seq_t*)realloc(seq_values, (seq_size + 1) * sizeof(seq_t));
    if (seq_values == NULL) { while (1); }
    seq_values[seq_size++] = (seq_t)n;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
/* Return the n-th term of A108219 (a(1) is the first term).  The
 * function is memoised; subsequent calls use the stored values. */
{
    if (n == 0) return 0;        /* order‑zero is undefined */
    init_sieve();

    while (seq_size < (size_t)n) {
        if (last_scanned >= MAX_SIEVE) {
            /* the pre‑computed range is exhausted; the required terms
             * have not been found, so further searching would exceed
             * the limit.  Return 0 to indicate failure. */
            return 0;
        }
        ++last_scanned;
        uint64_t s = sumfact[last_scanned];
        if (is_golden_semiprime(s)) {
            add_term(last_scanned);
        }
    }
    return seq_values[n - 1];
}
