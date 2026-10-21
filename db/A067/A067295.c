/*
 * A067295  Fourth column of triangle A028364.
 * https://oeis.org/A067295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A028364(n+3,3) a(n) = C(0)*C(n+3) + C(1)*C(n+2) +
 *            C(2)*C(n+1) + C(3)*C(n) where C(k) is the Catalan number
 *            A000108(k) a(n) =
 *            (3*(31*n³+163*n²+254*n+112)/(8*(2*n+1)*(2*n+3)*(2*n+5))) *
 *            C(n+3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *catalan = NULL;     /* array of Catalan numbers C(0),C(1),… */
static idx_t max_computed = 0;    /* largest index currently stored (C(max_computed) exists) */

/* make sure Catalan numbers up to index 'upto' are available */
static void ensure_catalan(idx_t upto)
{
    if (max_computed >= upto)
        return;                      /* already available */

    /* allocate or grow storage */
    if (!catalan) {
        /* first call: allocate space for upto+1 entries */
        catalan = (seq_t *)malloc((upto + 1) * sizeof(seq_t));
        if (!catalan)                /* allocation failed – caller will fail too */
            return;
        catalan[0] = 1;              /* C(0) = 1 */
        max_computed = 0;
    } else if (upto + 1 > (idx_t)0) {
        seq_t *tmp = (seq_t *)realloc(catalan, (upto + 1) * sizeof(seq_t));
        if (!tmp) return;            /* allocation failed – caller will fail too */
        catalan = tmp;
    }

    /* compute missing Catalan numbers, using C(m+1)=C(m)*(4*m+2)/(m+2) */
    for (idx_t m = max_computed; m < upto; ++m) {
        seq_t numer = catalan[m] * (seq_t)(4 * m + 2);
        seq_t denom = (seq_t)(m + 2);
        catalan[m + 1] = numer / denom;
    }
    max_computed = upto;
}

/* main sequence function required by the framework */
seq_t a(idx_t n)
{
    /* we need Catalan numbers up to C(n+3) */
    ensure_catalan(n + 3);

    /* a(n) = C(n+3) + C(n+2) + 2*C(n+1) + 5*C(n) */
    seq_t part0 = catalan[n + 3];
    seq_t part1 = catalan[n + 2];
    seq_t part2 = catalan[n + 1] * 2;
    seq_t part3 = catalan[n] * 5;

    return part0 + part1 + part2 + part3;
}
