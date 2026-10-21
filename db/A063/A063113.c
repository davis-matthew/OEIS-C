/*
 * A063113  a(1) = 1; a(n+1) = a(n) + product of nonzero digits of a(n) when
 *          written in base 3. But display sequence in base 10.
 * https://oeis.org/A063113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(n+1)=a(n)+prod_nonzero_base3(a(n))
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prod_nonzero_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/prod_nonzero_base3.h"

/* dynamic storage for sequence terms (0‑based index; a(1) at a[0]) */
static seq_t *seq_a       = NULL;
static size_t seq_a_cap   = 0;    /* allocated capacity               */
static size_t seq_a_len   = 0;    /* number of terms stored so far    */

/* enlarge the internal array to hold at least 'need' elements */
static void seq_ensure_capacity(size_t need)
{
    if (seq_a_cap >= need)
        return;
    size_t new_cap = seq_a_cap ? seq_a_cap << 1 : 8;
    if (new_cap < need)
        new_cap = need;
    /* realloc is safe for both C and C++ when cast from void* to seq_t* */
    seq_a = (seq_t *)realloc(seq_a, new_cap * sizeof(seq_t));
    seq_a_cap = new_cap;
}

/* compute a(n) (n is 1‑based) */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* undefined for n < 1 */

    /* initialise first term if necessary */
    if (seq_a_len == 0) {
        seq_ensure_capacity(1);
        seq_a[0] = (seq_t)1; /* a(1) = 1 */
        seq_a_len = 1;
    }

    /* compute missing terms up to n */
    size_t target = (size_t)n;    /* n is guaranteed to be <= size_t max */
    while (seq_a_len < target) {
        seq_ensure_capacity(seq_a_len + 1);
        seq_t last = seq_a[seq_a_len - 1];
        seq_a[seq_a_len] = last + prod_nonzero_base3(last);
        seq_a_len++;
    }

    return seq_a[n - 1];
}
