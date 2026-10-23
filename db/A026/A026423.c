/*
 * A026423  a(n) = least positive integer > a(n-1) and not a(i)*a(j) for
 *          1<=i<=j<=n, n >= 2.
 * https://oeis.org/A026423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A026423: a(1)=1, a(2)=3; for n>=3, a(n) is the smallest integer
    > a(n-1) that is not equal to a(i)*a(j) for any i,j with 1 ≤ i ≤ j < n.   */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static seq_t *terms = NULL;           /* stored sequence values */
static size_t terms_cap = 0;          /* capacity of terms */
static idx_t terms_len = 0;           /* number of terms stored */

static unsigned char *present = NULL; /* present[x] == 1 iff x occurs in terms */
static size_t present_cap = 0;        /* capacity of present array */

/* Grow an array of seq_t to at least 'need' elements. */
static void grow_terms(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!p) abort();
    terms = p;
    terms_cap = new_cap;
}

/* Grow present[] to at least 'need' indices. */
static void grow_present(size_t need)
{
    if (present_cap >= need) return;
    size_t new_cap = present_cap ? present_cap << 1 : 256;
    while (new_cap < need) new_cap <<= 1;
    unsigned char *p = (unsigned char *)realloc(present, new_cap);
    if (!p) abort();
    /* initialise new entries to 0 */
    memset(p + present_cap, 0, new_cap - present_cap);
    present = p;
    present_cap = new_cap;
}

/* Checks whether integer x is a product of two terms already in the
   sequence (both indices < current length). */
static bool is_product(seq_t x)
{
    for (idx_t k = 0; k < terms_len; ++k) {
        seq_t y = terms[k];
        if (y == 0) continue;                 /* safety */
        if (y > x / y) break;                 /* y*y > x -> larger y will also -->
                                                /* exceed */
        if (x % y == 0) {
            seq_t z = x / y;
            /* z must also be an existing term; use present array */
            if ((size_t)z < present_cap && present[(size_t)z])
                return true;
        }
    }
    return false;
}

/* Compute terms up to n (inclusive). */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;                     /* offset 1 means a(1) is first term */
    /* Initialise first two terms if necessary */
    if (terms_len == 0) {
        grow_terms(2);
        grow_present(4);                    /* indices 1 and 3 needed */
        terms[0] = (seq_t)1;
        terms[1] = (seq_t)3;
        present[1] = 1;
        present[3] = 1;
        terms_len = 2;
    }
    while (terms_len < n) {
        seq_t prev = terms[terms_len - 1];
        seq_t cand = prev + 1;
        for (;;) {
            if (!is_product(cand)) break;
            ++cand;
        }
        /* ensure storage large enough */
        grow_terms(terms_len + 1);
        grow_present((size_t)cand + 1);
        terms[terms_len] = cand;
        present[(size_t)cand] = 1;
        ++terms_len;
    }
}

/* public routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but safe */
    ensure_terms(n);
    return terms[n - 1];
}
