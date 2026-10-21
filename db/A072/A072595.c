/*
 * A072595  Numbers k such that A072594(k) = 0.
 * https://oeis.org/A072595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A072595(k) is the k‑th positive integer n such that the bitwise
 *            XOR of the prime factors of n that occur an odd number of times
 *            is zero. The function searches consecutively for such numbers,
 *            uses a sieve of smallest prime factors for fast factorisation
 *            and caches the results so calls are order‑in...
 * Range:     u8 n <= 15, u16 n <= 347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"

static idx_t *spf          = NULL;   /* smallest prime factor array   */
static size_t spf_size     = 0;      /* current maximum index stored */

static seq_t *terms        = NULL;   /* cached sequence values       */
static size_t terms_len    = 0;      /* number of cached entries      */
static size_t terms_cap    = 0;      /* capacity of terms array      */

/* ------------------------------------------------------------------ */
/* Ensure the SPF table contains data up to at least 'limit'.          */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_size)
        return;

    if (spf)
        free(spf);                   /* old table can be freed          */
    build_spf(limit, &spf);          /* builds new table up to limit    */
    spf_size = limit;
}

/* ------------------------------------------------------------------ */
/* Compute XOR of primes with odd multiplicity in the factorisation
   of 'n'.  The SPF array is used for fast factorisation.           */
static seq_t xor_odd_primes(idx_t n)
{
    seq_t xor_val = 0;
    while (n > 1) {
        idx_t p = spf[n];
        if (p == 0)                   /* n is prime if not in table */
            p = n;
        int parity = 0;
        while (n % p == 0) {
            n /= p;
            parity ^= 1;              /* toggle parity                */
        }
        if (parity)
            xor_val ^= (seq_t)p;
    }
    return xor_val;
}

/* ------------------------------------------------------------------ */
/* Main required function.                                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, guard clause       */

    static idx_t next_candidate = 4;   /* first possible term           */

    while (terms_len < n) {
        /* Make sure SPF table includes the candidate. */
        if ((size_t)next_candidate > spf_size)
            ensure_spf((idx_t)(next_candidate * 2 + 1024));

        /* Check the candidate. */
        if (xor_odd_primes(next_candidate) == 0) {
            /* Grow the cache if needed. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                terms = seq_realloc(terms, terms_cap, new_cap);
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
