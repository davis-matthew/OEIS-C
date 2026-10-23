/*
 * A157496  Composite numbers n where the largest digit m is equal to the sum
 *          of all digits of n except for one occurrence of m.
 * https://oeis.org/A157496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the n-th composite number whose largest digit m *
 *            satisfies m = sum of all digits of the number except one *
 *            occurrence of m. The sequence starts with 22, 33, 44,
 * Range:     u8 n <= 33, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static bool property(uint64_t n)
{
    if (n <= 1) return false;                /* 1 is not composite */
    if (is_prime_small((seq_t)n)) return false;   /* skip primes */

    uint64_t sum = 0;
    int maxd = 0;
    uint64_t x = n;
    while (x > 0) {
        int d = (int)(x % 10);
        sum += (uint64_t)d;
        if (d > maxd) maxd = d;
        x /= 10;
    }
    return (sum == 2u * (uint64_t)maxd);
}

/* Cache of the computed terms (1‑based).  terms[0] = a(1). */
static seq_t *terms = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;

static void grow_terms(size_t needed)
{
    if (term_cap >= needed) return;
    size_t new_cap = term_cap ? term_cap : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();   /* fail fast */
    terms = new_terms;
    term_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    /* Ensure we have at least n terms. */
    while (term_count < (size_t)n) {
        uint64_t candidate = (term_count == 0) ? 2 :
                             (uint64_t)terms[term_count - 1] + 1;
        /* Search for the next term. */
        while (!property(candidate)) candidate++;
        grow_terms(term_count + 1);
        terms[term_count] = (seq_t)candidate;
        term_count++;
    }
    return terms[(size_t)n - 1];
}
