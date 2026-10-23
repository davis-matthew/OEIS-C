/*
 * A107746  Numbers k such that the least prime factor of 6*k+1 > the least
 *          prime factor of 6*k-1, A107744(k) > A107745(k).
 * https://oeis.org/A107746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107746: a(n) is the n‑th integer k such that the least prime factor of
   6*k+1 is greater than the least prime factor of 6*k-1.  The function
   searches the integers in ascending order, using a simple sieve of
   primes for trial division. */

/* Standard headers; the types seq_t and idx_t are typedef'ed by the
   build system. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Prime sieve and lookup                                   */
/* ------------------------------------------------------------ */
static uint32_t *prime_list  = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;       /* number of primes stored */
static bool primes_ready = false;      /* one‑time init flag */

/* One‑time initialization of the prime list.  LIMIT is chosen so
   sqrt(6*k±1) for all k that will be tested for n≤10 000 is comfortably
   covered.  The value 200 000 guarantees >10 000 hits. */
static void init_primes(void)
{
    if (primes_ready) return;
    primes_ready = true;

    /* 200 000 is sufficient for the required n range. */
    const size_t LIMIT = 200000;

    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_composite) return;  /* allocation failure – give up */

    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (!is_composite[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p) {
                is_composite[q] = true;
            }
        }
    }

    /* Count primes first to allocate once. */
    for (size_t p = 2; p <= LIMIT; ++p) {
        if (!is_composite[p]) ++prime_cnt;
    }

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) {
        free(is_composite);
        return;  /* allocation failure – give up */
    }

    size_t idx = 0;
    for (size_t p = 2; p <= LIMIT; ++p) {
        if (!is_composite[p]) {
            prime_list[idx++] = (uint32_t)p;
        }
    }

    free(is_composite);
}

/* Return the least prime factor of n (n≥2).  If n is 0 or 1, return n. */
static uint32_t least_prime_factor(uint64_t n)
{
    if (n < 2) return (uint32_t)n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) return p;
    }
    /* n itself is prime */
    return (uint32_t)n;
}

/* ------------------------------------------------------------ */
/* Sequence function                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range, harmless */

    init_primes();

    uint64_t found = 0;
    uint64_t k = 1;

    while (found < (uint64_t)n) {
        uint64_t v1 = 6 * k + 1;
        uint64_t v2 = 6 * k - 1;

        uint32_t lpf1 = least_prime_factor(v1);
        uint32_t lpf2 = least_prime_factor(v2);

        if (lpf1 > lpf2) {
            ++found;
            if (found == (uint64_t)n)
                return (seq_t)k;
        }
        ++k;
    }

    return (seq_t)0;  /* unreachable, defensive */
}
