/*
 * A178404  Numbers such that the rounded up arithmetic mean of their digits
 *          equals their digital root.
 * https://oeis.org/A178404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search enumeration of integers whose ceiling of the arithmetic
   mean of their decimal digits equals their digital root. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"

/* cached terms, 0‑based index */
static seq_t *terms        = NULL;
/* allocated capacity */
static size_t  terms_cap   = 0;
/* number of terms stored */
static size_t  terms_cnt   = 0;
/* next candidate to test */
static seq_t  next_candidate = 0;

/* Ensure at least 'need' terms can be stored. */
static void ensure_capacity(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Return true if x satisfies the defining property. */
static bool good(seq_t x)
{
    seq_t S = digit_sum(x);          /* sum of decimal digits */
    int    d = digit_len(x);         /* number of decimal digits */

    /* ceil(S / d) = (S + d – 1) / d */
    seq_t k = (S + (seq_t)d - 1) / (seq_t)d;

    /* For a digital root k must be ≤ 9. */
    if (k > 9) return false;

    /* Digital root of x */
    int DR = 0;
    if (S != 0) {
        seq_t r = S % 9;
        DR = (int)(r == 0 ? 9 : r);
    }

    return (int)k == DR;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safe guard: offset 1 */

    size_t target = (size_t)n;                 /* number of terms wanted */

    /* generate terms until we have enough */
    while (terms_cnt < target) {
        seq_t x = next_candidate;
        if (good(x)) {
            ensure_capacity(terms_cnt + 1);
            terms[terms_cnt++] = x;
        }
        next_candidate = x + 1;
    }

    return terms[n - 1];
}
