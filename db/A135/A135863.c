/*
 * A135863  G.f. A(x) = 1 + 4*x*A(x)^(1/2); A(x) = 1 + 8*x^2 + 4*x*sqrt(1 +
 *          4*x^2).
 * https://oeis.org/A135863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 20, i32 n <= 36, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135863
   a(0) = 1,  a(1) = 4,  a(2) = 8
   For n odd (n = 2m+1, m ≥ 1):
        a(n) = (-1)^m * 8 * Catalan(m-1)
   For n even and n ≥ 4:  a(n) = 0
   Catalan numbers are generated multiplicatively:
        C_0 = 1
        C_k = C_{k-1} * 2 * (2*(k-1)+1) / (k+1)   for k ≥ 1
   The routine memoises Catalan values so that each term is computed in
   O(1) after an initial O(k) build. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the few exceptional initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* Even indices (n ≥ 4) produce zero */
    if ((n & 1) == 0) return (seq_t)0;

    /* For odd indices n = 2m+1 ≥ 3, let k = m-1 = (n-3)/2 */
    idx_t k = (n - 3) / 2;               /* k ≥ 0 */

    /* Lazily allocate (or grow) a cache of Catalan numbers */
    static seq_t *cat = NULL;
    static size_t cat_cap = 0;           /* capacity of the array */
    static size_t cat_size = 0;          /* number of computed elements */

    /* Number of Catalan numbers needed (index k inclusive) */
    size_t need = (size_t)k + 1;
    if (cat_cap < need) {
        /* Grow the allocation exponentially */
        size_t new_cap = cat_cap ? cat_cap : 1;
        while (new_cap < need) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cat, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();  /* allocation failure */
        cat = tmp;
        cat_cap = new_cap;
    }

    /* Compute missing Catalan numbers up to index k */
    for (size_t i = cat_size; i <= (size_t)k; ++i) {
        if (i == 0) {
            cat[0] = (seq_t)1;
        } else {
            /* C_i = C_{i-1} * 2 * (2*(i-1)+1) / (i+1) */
            seq_t numer = cat[i-1];
            numer *= (seq_t)2;
            numer *= ( (seq_t)2 * (seq_t)(i-1) + (seq_t)1 );
            cat[i] = numer / ( (seq_t)(i+1) );
        }
    }
    cat_size = need;

    /* Assemble the answer: a(n) = (-1)^k * 8 * C_k */
    seq_t res = cat[(size_t)k] * (seq_t)8;
    if ((k & 1) != 0) res = -res;  /* apply sign (-1)^k */
    return res;
}
