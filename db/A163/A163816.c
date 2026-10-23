/*
 * A163816  Integers with exactly 100 divisors.
 * https://oeis.org/A163816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163816: integers with exactly 100 divisors.
   The routine searches successive integers, counts their divisors
   via trial division with a pre‑computed prime list, and memoises the
   discovered terms.  It is fully lazy and works correctly for any
   call order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve – static, full of 9592 primes up to 100000 (≈10⁵).   */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 100000
/* How many primes <= 100000?  (known to be 9592) */
#define PRIME_COUNT 9592

static bool prime_mark[PRIME_LIMIT + 1];
static idx_t prime_list[PRIME_COUNT];
static size_t prime_used = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        prime_mark[i] = true;

    for (size_t i = 2; (idx_t)i * (idx_t)i <= PRIME_LIMIT; ++i)
        if (prime_mark[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                prime_mark[j] = false;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_mark[i]) prime_list[prime_used++] = (idx_t)i;

    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/*  Divisor count – trial division using primes up to sqrt(n).        */
/* ------------------------------------------------------------------ */
static unsigned long long tau_of(idx_t n)
{
    unsigned long long tau = 1;
    idx_t temp = n;
    for (size_t i = 0; i < prime_used; ++i) {
        idx_t p = prime_list[i];
        if ((unsigned __int128)p * p > temp) break;
        if (temp % p == 0) {
            unsigned int exp = 0;
            do { temp /= p; ++exp; } while (temp % p == 0);
            tau *= (exp + 1);
            if (tau > 100) return tau;        /* early exit */
        }
    }
    if (temp > 1) tau *= 2;          /* remaining prime factor */
    return tau;
}

/* ------------------------------------------------------------------ */
/*  Sequence memoisation – dynamic array of found terms.               */
/* ------------------------------------------------------------------ */
static seq_t *term_cache   = NULL;
static size_t cache_len    = 0;
static size_t cache_cap    = 0;
static idx_t next_candidate = 1;   /* integer to test next          */

static void expand_cache(idx_t need)
{
    if (cache_len >= (size_t)need) return;

    init_primes();                    /* ensure primes are ready      */

    while (cache_len < (size_t)need) {
        if (tau_of(next_candidate) == 100) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* out of memory */
                for (size_t i = 0; i < cache_len; ++i)
                    tmp[i] = term_cache[i];
                free(term_cache);
                term_cache = tmp;
                cache_cap = new_cap;
            }
            term_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point – returns the n‑th term (1‑based).             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    expand_cache(n);
    return term_cache[n - 1];
}
