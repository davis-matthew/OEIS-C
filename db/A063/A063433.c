/*
 * A063433  'Reverse and Add!' trajectory of 10577.
 * https://oeis.org/A063433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 11, u64 n <= 31, u128 n <= 76,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverse and Add! trajectory of 10577
 * Sequence is defined recursively:
 *   a(0) = 10577
 *   a(n) = a(n-1) + reverse(a(n-1))   for n > 0
 * where reverse(x) reads the base‑10 decimal representation of x backwards.
 *
 * The implementation caches all terms so that repeated calls with any
 * index order run in O(1) after the term has been generated once.
 */
#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *terms   = NULL;
    static size_t  cap    = 0;   /* allocated size */
    static size_t  len    = 0;   /* number of terms stored */

    /* initialise cache on first call */
    if (terms == NULL) {
        cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;          /* allocation failure */
        terms[0] = (seq_t)10577;
        len = 1;
    }

    /* grow array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        terms = tmp;
        cap   = new_cap;
    }

    /* compute terms up to index n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t prev = terms[i - 1];
        seq_t rev  = reverse_digits(prev);
        terms[i] = prev + rev;
    }
    len = (size_t)n + 1;

    return terms[(size_t)n];
}
