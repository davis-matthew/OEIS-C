/*
 * A038033  a(n) = A000166(n-1)*n*(n-1).
 * https://oeis.org/A038033
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000166(n-1) * n * (n-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *der_cache = NULL;    /* derangement values */
static size_t der_alloc  = 0;      /* allocated size of der_cache */
static size_t der_computed = 0;    /* number of derangements already filled */

/* Ensure der_cache contains derangements up to m = n-1 (inclusive) */
static void ensure_der_up_to(idx_t m)
{
    /* Need space for indices 0 .. m inclusive */
    size_t need = (size_t)m + 1;
    if (need <= der_alloc)
        return;

    /* Reallocate array to the required size */
    seq_t *tmp = (seq_t *)realloc(der_cache, need * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort the program (expected to never happen) */
        abort();
    }
    der_cache = tmp;
    der_alloc = need;

    /* Initialize the first entries if this is the first allocation */
    if (der_computed == 0) {
        der_cache[0] = (seq_t)1;      /* der[0] = 1 */
        der_computed = 1;
    }
    if (der_computed == 1 && need > 1) {
        der_cache[1] = (seq_t)0;      /* der[1] = 0 */
        der_computed = 2;
    }

    /* Compute remaining derangement values up to m */
    for (size_t i = der_computed; i < need; ++i) {
        /* der[i] = (i-1) * (der[i-1] + der[i-2]) */
        seq_t sum = der_cache[i - 1] + der_cache[i - 2];
        der_cache[i] = (seq_t)(i - 1) * sum;
    }
    der_computed = need;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;            /* definition only valid for n ≥ 3 */

    idx_t m = n - 1;                /* index for derangement sequence */
    ensure_der_up_to(m);

    seq_t der = der_cache[(size_t)m];
    return der * (seq_t)n * (seq_t)(n - 1);
}
