/*
 * A077535  Multiples of 6 with only prime digits (2, 3, 5 and 7).
 * https://oeis.org/A077535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A077535 - Multiples of 6 with only prime digits (2,3,5,7)
   a(n) = nth smallest integer that is a multiple of 6 and whose all decimal
   digits belong to {2,3,5,7}.  The sequence is obtained by enumerating all
   such numbers in increasing order and selecting the nth element.  The
   implementation builds the list lazily and keeps it cached so that
   subsequent calls are fast. */

#include <stdlib.h>
#include <stdint.h>

/* the build provides seq_t and idx_t types */
static seq_t *terms      = NULL;   /* array of generated terms      */
static idx_t terms_len   = 0;      /* number of stored terms        */
static idx_t terms_cap   = 0;      /* current capacity of array     */
static int   explored_len = 0;     /* maximum digit length generated */

static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return;            /* allocation failure – abort silently */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

static void generate_len(int len, seq_t cur)
{
    if (len == 0) {
        if ((cur % 6) == 0) push_term(cur);   /* only keep multiples of 6 */
        return;
    }
    static const int digs[4] = { 2, 3, 5, 7 };
    for (int i = 0; i < 4; ++i) {
        generate_len(len - 1, cur * 10 + digs[i]);
    }
}

static void ensure_terms(idx_t n)
{
    if (terms_len >= n) return;          /* already enough terms        */
    int len = explored_len + 1;
    while (terms_len < n) {
        generate_len(len, 0);
        ++len;
    }
    explored_len = len - 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1       */
    ensure_terms(n);
    return terms[n - 1];
}
