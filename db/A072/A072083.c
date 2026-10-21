/*
 * A072083  Numbers divisible by the 4th power of the sum of their digits in
 *          base 10.
 * https://oeis.org/A072083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A072083:
   numbers divisible by the fourth power of the sum of their digits in base 10. */

#include <stddef.h>
#include <stdlib.h>

/* Helper: compute the sum of decimal digits of an unsigned long long. */
static unsigned int digit_sum_ull(unsigned long long x)
{
    unsigned int sum = 0;
    while (x) {
        sum += (unsigned int)(x % 10ULL);
        x /= 10ULL;
    }
    return sum;
}

/* Dynamic array to cache found terms. */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_terms_cap(size_t need)
{
    if (need > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = new_terms;
        terms_cap = new_cap;
    }
}

/* Last candidate examined; persists across calls. */
static unsigned long long next_candidate = 1;
static int cache_initialized = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but keep safe */

    /* Initialize cache on first call. */
    if (!cache_initialized) {
        cache_initialized = 1;
        next_candidate = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        unsigned long long m = next_candidate;
        unsigned int sum = digit_sum_ull(m);
        unsigned long long d = (unsigned long long)sum;
        /* Compute sum^4: d = sum * sum * sum * sum */
        d = d * d * d * d;
        if (d != 0 && m % d == 0) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)m;
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
