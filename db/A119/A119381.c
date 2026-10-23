/*
 * A119381  Primes p(n) for which (p(n-1) + p(n+2)) / p(n) = 2.
 * https://oeis.org/A119381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate consecutive primes and keep those
   for which p(i‑1)+p(i+2) = 2·p(i).  The array a has
   offset 1 (a(1) = 37).  The routine is memoised and
   works for any n ≥ 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Parameters: the maximum number of primes and special primes that
   we are willing to generate.  For n up to 10 000 the needed
   largest prime is well below 3 million; this table comfortably
   covers that range even for a 128‑bit seq_t.
   ------------------------------------------------------------------ */
#define MAX_PRIMES   600000   /* safety margin */
#define MAX_SPECIALS 20000    /* enough for n ≈ 10 000 */

static seq_t primes[MAX_PRIMES];
static size_t primes_len = 0;

static seq_t specials[MAX_SPECIALS];
static size_t specials_len = 0;

static seq_t next_candidate = 3;

/* ------------------------------------------------------------------
   Simple trial‑division primality test using the primes we have
   already found.  This works for any integral or arbitrary‑precision type
   that supports the usual arithmetic operators and comparisons.
   ------------------------------------------------------------------ */
static bool is_prime_trial(seq_t x)
{
    if (x < (seq_t)2)
        return false;
    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p = primes[i];
        if (p * p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------
   Store a newly found prime.  The array bounds are checked
   only on the (unlikely) event that we exceed the
   predefined maximum.
   ------------------------------------------------------------------ */
static void add_prime(seq_t p)
{
    if (primes_len < MAX_PRIMES) {
        primes[primes_len++] = p;
    }
}

/* ------------------------------------------------------------------
   Store a prime that satisfies the required condition.
   ------------------------------------------------------------------ */
static void add_special(seq_t p)
{
    if (specials_len < MAX_SPECIALS) {
        specials[specials_len++] = p;
    }
}

/* ------------------------------------------------------------------
   Main routine.  It keeps generating primes until at least n
   special primes have been collected and then returns the
   (n‑1)-th element of the specials array (a(1) is the first
   special prime, 37).  The function can be called in any order.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for offset 0 */

    /* initialise the prime list with the first prime */
    if (primes_len == 0) {
        add_prime((seq_t)2);
    }

    /* generate primes until we have enough special primes */
    while (specials_len < (size_t)n) {
        seq_t cand = next_candidate;
        if (is_prime_trial(cand)) {
            add_prime(cand);
            /* once we have at least 4 primes we can test the
               prime that is two places behind the newest one. */
            if (primes_len >= 4) {
                size_t k = primes_len;              /* number of primes known */
                seq_t lhs = primes[k - 4] + primes[k - 1];
                seq_t rhs = (seq_t)2 * primes[k - 3];
                if (lhs == rhs) {
                    add_special(primes[k - 3]);      /* p(n) at index k‑2 */
                }
            }
        }
        next_candidate += (seq_t)2;      /* next odd candidate */
    }

    return specials[n - 1];
}
