/*
 * A062544  a(n) = n plus sum of previous three terms.
 * https://oeis.org/A062544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + a(n-1) + a(n-2) + a(n-3) Initial values: a(0)=0,
 *            a(1)=1, a(2)=3
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *terms = NULL;     /* array of computed values */
static size_t   len   = 0;      /* number of elements stored */
static size_t   cap   = 0;      /* allocated capacity */

/* Ensure the array has at least [needed] elements (0‑based indexing). */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    size_t new_cap = cap ? cap << 1 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    /* In case of allocation failure, the program will abort; this is
       acceptable for a sequence generator used in a controlled environment. */
    terms = new_terms;
    cap   = new_cap;
}

/* Compute a(n).  The function is thread‑unsafe but satisfies the
   requirements for the build system (static state may exist). */
seq_t a(idx_t n)
{
    /* Initialise for n < 3 if necessary */
    if (len < 3) {
        ensure_capacity(3);
        if (len == 0) {
            terms[0] = (seq_t)0;
            terms[1] = (seq_t)1;
            terms[2] = (seq_t)3;
            len = 3;
        }
    }

    if ((idx_t)n < (idx_t)len)   /* already computed */
        return terms[n];

    /* Ensure capacity for n+1 elements */
    ensure_capacity((size_t)n + 1);

    /* Compute values from current len up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)i;
        if (i >= 1) sum += terms[i - 1];
        if (i >= 2) sum += terms[i - 2];
        if (i >= 3) sum += terms[i - 3];
        terms[i] = sum;
    }
    len = n + 1;
    return terms[n];
}
