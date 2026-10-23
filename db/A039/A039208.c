/*
 * A039208  Numbers whose base-11 representation has the same number of 8's
 *          and 10's.
 * https://oeis.org/A039208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 178, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th non‑negative integer whose base‑11 representation
 * contains an equal number of digit 8’s and digit 10’s.  The function memoises
 * terms so that calls may be made in any order.  It is valid C11 and C++17. */
#include <stdlib.h>
#include <stdbool.h>

/* dynamic cache of found terms */
static seq_t *terms = NULL;      /* array of known terms           */
static size_t terms_cap = 0;     /* allocated capacity             */
static size_t terms_len = 0;     /* number of terms actually stored=terms_len */
static seq_t next_candidate = 0; /* next integer to test           */

/* Return true if x has equal numbers of 8’s and 10’s in its base‑11 representation. */
static bool good(seq_t x)
{
    unsigned cnt8  = 0;
    unsigned cnt10 = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 11);
        if (d == 8)   ++cnt8;
        else if (d == 10) ++cnt10;
        x /= 11;
    }
    return cnt8 == cnt10;
}

/* Grow the terms array to at least new_cap entries. */
static void grow_terms(size_t new_cap)
{
    size_t cap = (terms_cap ? terms_cap << 1 : 64);
    if (cap < new_cap) cap = new_cap;
    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure – terminate */
    terms = tmp;
    terms_cap = cap;
}

/* Generate terms until we have at least target terms stored. */
static void generate_up_to(size_t target)
{
    seq_t val = next_candidate;
    while (terms_len < target) {
        if (good(val)) {
            if (terms_len + 1 > terms_cap) grow_terms(terms_len + 1);
            terms[terms_len++] = val;
        }
        ++val;
    }
    next_candidate = val;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, but safe guard */
    size_t sn = (size_t)n;
    if (sn <= terms_len) return terms[sn - 1];
    generate_up_to(sn);
    return terms[sn - 1];
}
