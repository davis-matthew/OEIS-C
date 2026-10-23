/*
 * A152073  a(n) = largest prime < prime(n) such that prime(n) - a(n) is a
 *          power of 2, where prime(n) is the n-th prime; a(n) = 0 if no such
 *          prime exists.
 * https://oeis.org/A152073
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = largest prime < prime(n) such that prime(n) – a(n) is a
 *            power of 2. If no such prime exists, a(n) = 0. The routine keeps
 *            a memoised list of primes so that calls can come in any order
 * Range:     u8 n <= 56, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the prime list */
static uint64_t *primelist = NULL;   /* array of primes */
static size_t   primecnt   = 0;      /* number of primes currently stored */
static idx_t    prime_limit = 0;     /* largest number that has been sieved */

/* ========== helper functions ========== */

/* Test whether x is a non‑zero power of two (works for seq_t). */
static bool is_power_of_two(seq_t x)
{
    return x != 0 && ((x & (x - 1)) == 0);
}

/* Generate all primes ≤ limit and store them in primelist. */
static void generate_primes_upto(idx_t limit)
{
    if (limit < 2) limit = 2;                 /* nothing below 2 */

    /* Allocate a bit array for the sieve */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)limit + 1);
    if (!is_prime) return;                   /* allocation failed – caller will ignore */

    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Eratosthenes sieve */
    for (size_t p = 2; p * p <= (size_t)limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= (size_t)limit; m += p)
                is_prime[m] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate new prime list */
    uint64_t *newlist = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!newlist) {
        free(is_prime);
        return;                               /* allocation failed */
    }

    /* Fill the new list */
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) newlist[idx++] = (uint64_t)i;

    free(is_prime);
    free(primelist);                          /* replace old list */
    primelist = newlist;
    primecnt   = cnt;
    prime_limit = limit;
}

/* Ensure that primelist contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (primecnt >= (size_t)n) return;

    idx_t limit = prime_limit > 0 ? prime_limit << 1 : 100; /* start with 100 */
    while ((size_t)n > primecnt) {
        generate_primes_upto(limit);
        if (primecnt >= (size_t)n) break;
        limit <<= 1;                          /* double the bound */
    }
}

/* ========== main sequence function ========== */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* value below the offset */

    ensure_primes(n);                        /* make sure prime(n) is available */

    if (primecnt < (size_t)n) return 0;       /* guard against allocation failure */

    uint64_t prime_n = primelist[n - 1];      /* prime(n) */

    /* Backward scan for the largest prime < prime_n whose difference is a power of 2 */
    for (idx_t i = n - 2;; --i) {
        seq_t diff = (seq_t)(prime_n - primelist[i]);   /* difference fits in seq_t */
        if (is_power_of_two(diff))
            return (seq_t)primelist[i];
        if (i == 0) break;                    /* reached the smallest prime (2) */
    }

    return 0;                                 /* no suitable prime found */
}
