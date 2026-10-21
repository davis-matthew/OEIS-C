/*
 * A239289  Numbers that are not the product of three (not necessarily
 *          distinct) primes.
 * https://oeis.org/A239289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer that is not the product of three (not
   necessarily distinct) primes.  It is computed by iterating through the
   integers and using a sieve for the smallest prime factor.  The function
   works for any 64‑bit unsigned index `idx_t` and any unsigned `seq_t`
   (including arbitrary‑precision types). */

#include <stdbool.h>
#include <stdlib.h>

/* 64‑bit index type */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Sequence cache (1‑based indexing) */
static seq_t *seq_cache = NULL;
static idx_t seq_cap  = 0;
static idx_t seq_len  = 0;

/* Largest integer that has been processed so far */
static idx_t processed = 0;

/* Extend the smallest‑prime‑factor table up to at least `limit`. */
static void ensure_spf(idx_t limit) {
    if (spf_limit >= limit)
        return;
    if (spf != NULL)
        free(spf);
    spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf)
        return;                     /* allocation failure – should not happen */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;
    if (limit >= 1)
        spf[1] = 1;
    for (idx_t p = 2; p <= limit; ++p) {
        if (spf[p] == 0) {                /* p is prime */
            spf[p] = p;
            if (p <= limit / p) {         /* avoid overflow in p*p */
                for (idx_t j = p * p; j <= limit; j += p)
                    if (spf[j] == 0)
                        spf[j] = p;
            }
        }
    }
    spf_limit = limit;
}

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache(idx_t need) {
    if (seq_cap >= need)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_cache = (seq_t *)realloc(seq_cache, (size_t)new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                       /* undefined for n == 0 */

    while (seq_len < n) {
        /* Next integer to examine. */
        idx_t val = processed + 1;

        /* Ensure we have spf values for this integer. */
        if (val > spf_limit)
            ensure_spf(val * 2 + 10);    /* safety margin */

        ++processed;

        /* Count prime factors with multiplicity (Ω). */
        idx_t x = processed;
        idx_t omega = 0;
        while (x > 1) {
            ++omega;
            x /= spf[x];
            if (x == 0) break;           /* protect against spf[0] misuse */
        }

        /* IfOmega != 3, we record the number. */
        if (omega != 3) {
            ensure_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)processed;
        }
    }
    return seq_cache[n - 1];
}
