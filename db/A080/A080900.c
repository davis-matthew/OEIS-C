/*
 * A080900  a(1)=1; for n>1, a(n)=a(n-1)-2 if n is already in the sequence,
 *          a(n)=a(n-1)+5 otherwise.
 * https://oeis.org/A080900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 9469, u32 n <= 9469, u64 n <= 9469,
 *            u128 n <= 9469, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080900
   a(1) = 1
   a(n) = a(n-1) – 2 if the integer n already occurs in the sequence
           a(n-1) + 5 otherwise
   Offset 1   (first term is a(1))
   The code builds the sequence on demand and caches previously
   computed terms.  Membership testing is performed by an expanding
   linear array of the values that have appeared so far. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached sequence values, indexed from 0 (a(1) is terms[0]) */
static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static idx_t terms_max   = 0;      /* number of terms currently stored */

/* List of all values that have appeared in the sequence (for membership test) */
static seq_t *seen      = NULL;
static size_t seen_cap  = 0;
static size_t seen_cnt  = 0;

/* Ensure that `terms` can hold at least N elements. */
static void terms_reserve(size_t N)
{
    if (terms_cap >= N) return;
    size_t cap = terms_cap ? terms_cap << 1 : 64;
    while (cap < N) cap <<= 1;
    seq_t *new_terms = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!new_terms) abort();
    if (terms) {
        for (size_t i = 0; i < terms_cap; ++i) new_terms[i] = terms[i];
        free(terms);
    }
    terms = new_terms;
    terms_cap = cap;
}

/* Ensure that `seen` can hold at least N elements. */
static void seen_reserve(size_t N)
{
    if (seen_cap >= N) return;
    size_t cap = seen_cap ? seen_cap << 1 : 64;
    while (cap < N) cap <<= 1;
    seq_t *new_seen = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!new_seen) abort();
    if (seen) {
        for (size_t i = 0; i < seen_cap; ++i) new_seen[i] = seen[i];
        free(seen);
    }
    seen = new_seen;
    seen_cap = cap;
}

/* Append a value to the list of seen values. */
static void seen_insert(seq_t v)
{
    seen_reserve(seen_cnt + 1);
    seen[seen_cnt++] = v;
}

/* Test whether a value has already appeared in the sequence. */
static bool seen_contains(seq_t v)
{
    for (size_t i = 0; i < seen_cnt; ++i)
        if (seen[i] == v) return true;
    return false;
}

/* Public function computing a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* values before offset are undefined */

    /* First call: initialise storage with a(1) */
    if (terms_max == 0) {
        terms_reserve(1);
        terms[0] = 1;
        terms_max = 1;
        seen_reserve(1);
        seen_insert(1);
    }

    /* If the requested term has already been computed, return it. */
    if (n <= terms_max) return terms[n - 1];

    /* Grow the terms array to accommodate up to n. */
    terms_reserve(n);
    seq_t prev = terms[terms_max - 1];

    for (idx_t i = terms_max + 1; i <= n; ++i) {
        seq_t next;
        if (seen_contains((seq_t)i))
            next = prev - 2;
        else
            next = prev + 5;
        prev = next;
        terms[i - 1] = next;
        seen_insert(next);
    }
    terms_max = n;
    return terms[n - 1];
}
