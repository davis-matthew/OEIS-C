/*
 * A076814  Integer averages of five successive primes squared,
 *          (prime(n)^2+prime(n+1)^2+prime(n+2)^2+prime(n+3)^2+prime(n+4)^2)/5,
 *          for some n.
 * https://oeis.org/A076814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (prime(n)^2+prime(n+1)^2+prime(n+2)^2+prime(n+3)^2+prime(n+4)^2)/5
 *            when the numerator is divisible by 5. The sequence is indexed
 *            from 1, with a(1) the first integer average of five successive
 *            primes squared
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/next_prime.h"

/* dynamic array helper – ensure capacity */
static void *ensure_cap(void *arr, size_t *cap, size_t item_size, size_t min_cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 128;
        while (new_cap < min_cap) new_cap <<= 1;
        arr = realloc(arr, new_cap * item_size);
        *cap = new_cap;
    }
    return arr;
}

/* global state */
static idx_t *primes = NULL;      /* all primes generated so far */
static size_t primes_len = 0, primes_cap = 0;

static seq_t *terms = NULL;       /* valid sequence terms */
static size_t terms_len = 0, terms_cap = 0;

static idx_t last_prime = 0;      /* last prime generated */

/* generate primes until we have at least 'need' primes */
static void ensure_primes(size_t need)
{
    while (primes_len < need) {
        idx_t p;
        if (last_prime == 0)          /* first prime */
            p = next_prime(2);
        else
            p = next_prime(last_prime + 1);
        last_prime = p;

        primes = (idx_t *)ensure_cap(primes, &primes_cap, sizeof(idx_t), primes_len + 1);
        primes[primes_len++] = p;
    }
}

/* extend the sequence until we have at least 'need' terms */
static void ensure_terms(idx_t need)
{
    while (terms_len < need) {
        /* need at least 5 primes to form a group */
        if (primes_len < 5)
            ensure_primes(5);

        /* compute average of the last 5 primes */
        seq_t sum = 0;
        for (idx_t i = primes_len - 5; i < primes_len; ++i) {
            seq_t sq = (seq_t)primes[i] * (seq_t)primes[i];
            sum += sq;
        }

        if ((sum % 5) == 0) {
            seq_t term = sum / 5;
            terms = (seq_t *)ensure_cap(terms, &terms_cap, sizeof(seq_t), terms_len + 1);
            terms[terms_len++] = term;
        }

        /* add next prime to advance */
        ensure_primes(primes_len + 1);
    }
}

/* required public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset<1; return 0 */
    ensure_terms(n);
    return terms[n - 1];            /* offset 1 */
}
