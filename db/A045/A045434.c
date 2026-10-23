/*
 * A045434  Primes congruent to {3, 6} mod 7.
 * https://oeis.org/A045434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 3 or 6 (mod 7)
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by on‑demand simple sieve – no closed form */

#include <stdlib.h>
#include <stdint.h>

/* static storage for primes and qualifying primes */
static idx_t *primes      = NULL;   /* all primes found so far      */
static size_t primes_cap  = 0;      /* capacity of primes[]        */
static size_t primes_cnt  = 0;      /* number of primes stored     */

static seq_t *qual_primes = NULL;   /* primes ≡ 3 or 6 (mod 7)      */
static size_t qual_cap    = 0;      /* capacity of qual_primes[]   */
static size_t qual_cnt    = 0;      /* number of qualifying primes */

/* grow the primes array, if required */
static void ensure_prime_cap(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes_cap = new_cap;
    }
}

/* grow the qualifying primes array, if required */
static void ensure_qual_cap(size_t needed)
{
    if (qual_cap < needed) {
        size_t new_cap = qual_cap ? qual_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        qual_primes = (seq_t *)realloc(qual_primes, new_cap * sizeof(seq_t));
        qual_cap = new_cap;
    }
}

/* return true if x is a prime – tested against the primes array */
static int is_prime_candidate(idx_t x)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Generate primes until we have at least n qualifying primes
 * (n is 1‑based index) */
static void extend_to_n(idx_t n)
{
    if (n == 0) return;          /* undefined – defensive */

    /* Ensure some initial space for primes */
    ensure_prime_cap(64);
    ensure_qual_cap(64);

    idx_t candidate = (primes_cnt > 0) ? primes[primes_cnt - 1] + 1 : 2;

    while (qual_cnt < n) {
        /* skip even numbers greater than 2 */
        if (candidate == 2 || (candidate & 1ULL)) {
            if (is_prime_candidate(candidate)) {
                /* store in primes array */
                ensure_prime_cap(primes_cnt + 1);
                primes[primes_cnt++] = candidate;

                /* check congruence condition */
                idx_t rem = candidate % 7;
                if (rem == 3 || rem == 6) {
                    ensure_qual_cap(qual_cnt + 1);
                    qual_primes[qual_cnt++] = (seq_t)candidate;
                }
            }
        }
        ++candidate;
    }
}

/* seq_t a(idx_t n) – public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, defensive */
    extend_to_n(n);
    return qual_primes[n - 1];
}
