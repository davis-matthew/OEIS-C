/*
 * A088608  a(1) = 1, a(n) = prime(n) if n is composite else a(n) =
 *          composite(n).
 * https://oeis.org/A088608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime(n) if n is composite, otherwise a(n) = composite(n); a(1)=1.
   The function uses lazy trial‑division to grow the prime and composite
   tables as needed.  It is safe for any n>=1 and retains state across
   calls. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *primes      = NULL;   /* list of primes, size primes_cnt    */
static size_t primes_cnt   = 0;
static size_t primes_cap   = 0;
static idx_t  last_prime_checked = 1; /* last number tested for primality */

static seq_t *composites  = NULL;   /* list of composites, size composites_cnt */
static size_t composites_cnt  = 0;
static size_t composites_cap  = 0;
static idx_t  last_comp_checked = 3; /* last number tested for composite */

/* Ensure we have at least n primes in the table. */
static void ensure_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n)
        return;

    if (primes_cap == 0) {
        primes_cap = 64;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
    } else if (primes_cap < (size_t)n) {
        size_t new_cap = primes_cap * 2;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        primes = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        primes_cap = new_cap;
    }

    for (idx_t x = last_prime_checked + 1; primes_cnt < (size_t)n; ++x) {
        bool is_p = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes[i];
            if (p * p > x) break;
            if (x % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            primes[primes_cnt++] = (seq_t)x;
        }
        last_prime_checked = x;
    }
}

/* Ensure we have at least n composites in the table. */
static void ensure_composites(idx_t n)
{
    if (composites_cnt >= (size_t)n)
        return;

    if (composites_cap == 0) {
        composites_cap = 64;
        composites = (seq_t *)malloc(composites_cap * sizeof(seq_t));
    } else if (composites_cap < (size_t)n) {
        size_t new_cap = composites_cap * 2;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        composites = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
        composites_cap = new_cap;
    }

    for (idx_t x = last_comp_checked + 1; composites_cnt < (size_t)n; ++x) {
        if (x < 4) continue;          /* 1,2,3 are neither prime nor composite for our sequence */
        if (x == 1) continue;
        bool prime = false;
        /* quick test using current prime list, extend primes if necessary */
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes[i];
            if (p * p > x) break;
            if (x % p == 0) { prime = false; goto end_check; }
        }
        /* if we exhausted the prime list, extend it enough to test */
        if (primes_cnt == 0 || primes[primes_cnt - 1] * primes[primes_cnt - 1] <= x) {
            ensure_primes((idx_t)((x * (seq_t)3) / 2) + 10); /* overshoot a bit */
            for (size_t i = 0; i < primes_cnt; ++i) {
                seq_t p = primes[i];
                if (p * p > x) break;
                if (x % p == 0) { prime = false; goto end_check; }
            }
        }
        prime = true; /* if not divisible by any known prime up to sqrt(x) */
        end_check:
        if (!prime) {
            composites[composites_cnt++] = (seq_t)x;
        }
        last_comp_checked = x;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Determine if n is composite.  For that we need primes up to n. */
    ensure_primes(n);
    bool n_is_prime = true;
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) { n_is_prime = false; break; }
    }

    if (!n_is_prime) {   /* n composite: return prime(n) */
        return primes[n - 1];
    } else {              /* n prime: return composite(n) */
        ensure_composites(n);
        return composites[n - 1];
    }
}
