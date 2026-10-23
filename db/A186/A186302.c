/*
 * A186302  a(n) = ( A007522(n)-1 )/2.
 * https://oeis.org/A186302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A007522(n)-1)/2 (A007522 is the n-th prime ≡ 7 (mod 8))
 * Range:     u8 n <= 14, u16 n <= 2275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *all_primes   = NULL;   /* all primes seen so far */
static idx_t     all_cnt      = 0;
static size_t    all_cap      = 0;

static uint64_t *spec_primes  = NULL;   /* primes ≡ 7 (mod 8) */
static idx_t     spec_cnt     = 0;
static size_t    spec_cap     = 0;

static uint64_t  next_candidate = 2;     /* next integer to test for primality */

/* Ensure we have space for one more prime in the ‘all’ table. */
static void ensure_all_cap(void)
{
    if (all_cnt < all_cap)
        return;
    size_t new_cap = all_cap ? all_cap * 2 : 16;
    uint64_t *ptr = (uint64_t *)realloc(all_primes, new_cap * sizeof(uint64_t));
    if (!ptr)
        abort();
    all_primes = ptr;
    all_cap    = new_cap;
}

/* Ensure we have space for one more prime in the ‘spec’ table. */
static void ensure_spec_cap(void)
{
    if (spec_cnt < spec_cap)
        return;
    size_t new_cap = spec_cap ? spec_cap * 2 : 16;
    uint64_t *ptr = (uint64_t *)realloc(spec_primes, new_cap * sizeof(uint64_t));
    if (!ptr)
        abort();
    spec_primes = ptr;
    spec_cap    = new_cap;
}

/* Trial division primality test using the primes found so far. */
static int is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return 0;
    for (idx_t i = 0; i < all_cnt; ++i) {
        uint64_t p = all_primes[i];
        if (p > x / p)          /* p*p > x  without overflow */
            break;
        if (x % p == 0)
            return 0;
    }
    return 1;
}

/* Grow the tables until we have at least n primes ≡ 7 (mod 8). */
static void expand_to(idx_t n)
{
    while (spec_cnt < n) {
        /* Skip even numbers greater than 2. */
        if (next_candidate > 2 && (next_candidate & 1ULL) == 0ULL) {
            next_candidate += 1;
            continue;
        }

        if (is_prime_uint64(next_candidate)) {
            /* Add to all_primes. */
            ensure_all_cap();
            all_primes[all_cnt++] = next_candidate;

            /* If ≡ 7 (mod 8) store in spec_primes. */
            if ((next_candidate & 7ULL) == 7ULL) {
                ensure_spec_cap();
                spec_primes[spec_cnt++] = next_candidate;
            }
        }
        next_candidate += 1;
    }
}

/* Core sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for index < 1, but harmless */

    expand_to(n);               /* guarantee we have the n‑th prime ≡ 7 (mod 8) */

    seq_t p = (seq_t)spec_primes[n - 1];
    return (p - 1) / 2;
}
