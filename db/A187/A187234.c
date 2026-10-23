/*
 * A187234  Numbers such that Sum_{i<j, i,j =1..r} d(i)*d(j) is a prime
 *          number, where the d(i) are digits of n (n = concatenation of d(1),
 *          ..., d(r)).
 * https://oeis.org/A187234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   get_digits, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A187234: numbers whose digit‑pair products sum to a prime.
   Compute by scanning successive integers, testing the predicate
   S = ∑_{i<j} d_i d_j, where d_i are the base‑10 digits of the
   candidate.  The value of S is S = ((∑d_i)² – ∑d_i²)/2.  The XOR of
   S must be prime.  The sequence is generated lazily; for each
   request a(n) the routine keeps a cache of already found terms.   */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_digits.h"      /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */
#include "../utils/isPrimeTrial.h"    /* bool isPrimeTrial(seq_t n); */

static seq_t *cache = NULL;      /* Holds the sequence values found so far   */
static size_t cache_len = 0;     /* Number of values in the cache           */
static size_t cache_cap = 0;     /* Allocated capacity of the cache         */
static idx_t last_tested = 0;    /* Largest integer already tested         */

/* Ensure the cache has room for at least 'need' elements.
   Reallocate with twice the current capacity or at least 8 entries. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort(); /* unrecoverable allocation failure */
    cache = newp;
    cache_cap = new_cap;
}

/* Test whether integer x satisfies the definition of the sequence. */
static bool qualifies(seq_t x)
{
    /* Get the decimal digits of x.  Digits are stored little‑endian. */
    uint8_t digits[64];          /* plenty of space for any number that fits */
    size_t dl = 0;
    get_digits(x, digits, &dl);

    /* Compute sum of digits and sum of squares of digits. */
    seq_t sum = 0;
    seq_t sumsq = 0;
    for (size_t i = 0; i < dl; ++i) {
        seq_t d = digits[i];
        sum += d;
        sumsq += d * d;
    }

    /* Compute pairwise product sum S = ((sum^2 - sumsq) / 2). */
    seq_t pair_sum = ((sum * sum - sumsq) / (seq_t)2);

    /* Return true iff pair_sum is a prime number. */
    return isPrimeTrial(pair_sum);
}

/* Return the nth element of A187234 (1‑based). */
seq_t a(idx_t n)
{
    /* Ensure we have at least n elements in the cache. */
    while (cache_len < n) {
        /* Test the next integer. */
        idx_t candidate = last_tested + 1;
        last_tested = candidate;
        seq_t cand_val = (seq_t)candidate;
        if (qualifies(cand_val)) {
            /* Append to cache. */
            grow_cache(cache_len + 1);
            cache[cache_len++] = cand_val;
        }
    }
    return cache[n - 1];
}
