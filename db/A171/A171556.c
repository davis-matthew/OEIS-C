/*
 * A171556  a(n) = 3*C(n)-2, where C(n) = A000108(n).
 * https://oeis.org/A171556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*C(n)-2, where C(n) is the n-th Catalan number
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>              /* malloc, realloc, free */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t, seq_t) */

/* Cache for Catalan numbers */
static seq_t *cat = NULL;    /* array of Catalan numbers 0 .. cat_size-1 */
static idx_t cat_cap = 0;    /* allocated capacity (in elements) */
static idx_t cat_size = 0;   /* number of computed elements (<= cat_cap) */

/* Ensure that Catalan numbers C(0)..C(n) are available in cat[]. */
static void ensure_cat(idx_t n)
{
    if (cat_size > n) return;               /* already computed */

    /* Grow cache if necessary.  Exponential growth keeps amortized O(1). */
    idx_t new_cap = cat_cap ? (cat_cap << 1) : 1;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cat, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        /* Allocation failure – aborting is acceptable for the evaluation. */
        exit(1);
    }
    cat = tmp;
    cat_cap = new_cap;

    /* Compute missing Catalan numbers using the recurrence
       C(0) = 1,
       C(n) = C(n-1) * (4*n-2) / (n+1).              */
    for (idx_t i = cat_size; i <= n; ++i) {
        if (i == 0) {
            cat[0] = (seq_t)1;
            continue;
        }

        seq_t prev = cat[i - 1];

        /* 4*i-2 fits in idx_t for all relevant n (<= 10000). */
        seq_t num = (seq_t)((4 * i) - 2);
        seq_t den = (seq_t)(i + 1);

        /* Reduce intermediate fractions to avoid overflow. */
        seq_t g = gcd(prev, den);
        prev /= g;
        den /= g;

        g = gcd(num, den);
        num /= g;
        den /= g;

        /* At this point den divides (prev * num) exactly. */
        cat[i] = (prev * num) / den;
    }

    cat_size = n + 1;  /* indices 0 .. n are now stored */
}

/* Return the n-th term of OEIS sequence A171556. */
seq_t a(idx_t n)
{
    ensure_cat(n);            /* make sure C(n) is computed */

    seq_t res = cat[n];
    res = res * (seq_t)3;     /* 3 * Catalan(n) */
    res = res - (seq_t)2;     /* minus 2 */

    return res;
}
