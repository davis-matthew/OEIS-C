/*
 * A057529  Numbers n with property that n is divisible by the number of
 *          divisors of n and by the sum of the digits of n.
 * https://oeis.org/A057529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = numbers n such that n % τ(n) == 0 && n % digit_sum(n) ==
 *            0
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static data for memoisation                                         */
/* ------------------------------------------------------------------ */
static seq_t  *terms        = NULL;   /* collected terms          */
static size_t  terms_cap    = 0;      /* capacity of terms array  */
static size_t  terms_len    = 0;      /* number of stored terms   */

static uint64_t checked     = 1;      /* last number examined     */

static uint64_t *primes      = NULL;   /* list of primes found so far */
static size_t   primes_cap   = 0;      /* capacity of primes array */
static size_t   primes_len   = 0;      /* number of known primes   */
static uint64_t prime_last_checked = 1;/* highest number tested for primes */


/* Extend primes array up to at least `limit` (inclusive) using trial division.
   The primes list grows only as needed; already discovered primes are reused. */
static void extend_primes_up_to(uint64_t limit)
{
    if (limit <= prime_last_checked)
        return;

    for (uint64_t x = prime_last_checked + 1; x <= limit; ++x) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > x)
                break;
            if (x % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 32;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp)
                    return;          /* silently abort on OOM */
                primes = tmp;
            }
            primes[primes_len++] = x;
        }
    }
    prime_last_checked = limit;
}

/* Compute number of divisors τ(n) using the primes list. */
static seq_t tau_from_uint64(uint64_t n)
{
    seq_t tau = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp)
            break;
        uint64_t e = 0;
        while (tmp % p == 0) {
            ++e;
            tmp /= p;
        }
        if (e > 0)
            tau *= (e + 1);
    }
    if (tmp > 1)       /* remaining prime factor */
        tau *= 2;
    return tau;
}

/* Digit sum of a 64‑bit unsigned integer. */
static uint64_t digit_sum_uint64(uint64_t n)
{
    uint64_t s = 0;
    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

/* Main entry point required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for 0, just guard */

    /* initialise storage on first call */
    if (terms_len == 0) {
        terms_cap = 32;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms)
            return 0;                /* OOM */
        terms[0] = (seq_t)1;
        terms_len = 1;
        checked = 1;
    }

    while (terms_len < (size_t)n) {
        ++checked;

        /* ensure primes list covers up to √checked, which is <= checked */
        extend_primes_up_to(checked);

        seq_t tau = tau_from_uint64(checked);
        uint64_t dsum = digit_sum_uint64(checked);

        if (checked % (uint64_t)tau == 0 && checked % dsum == 0) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;          /* OOM */
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)checked;
        }
    }
    return terms[n - 1];
}
