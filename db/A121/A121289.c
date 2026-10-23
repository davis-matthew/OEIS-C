/*
 * A121289  a(n) = n/(largest triangular number dividing n).
 * https://oeis.org/A121289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / (largest triangular number dividing n)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 1, ... */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of triangular numbers T_k = k*(k+1)/2, for k >= 1.
   triangular[i] == T_{i+1}. */
static seq_t *triangular = NULL;
static size_t tri_len = 0;   /* number of stored triangular values   */
static size_t tri_cap = 0;   /* capacity of the triangular array    */

/* Ensure that triangular numbers up to limit (inclusive) are present. */
static void ensure_triangular(idx_t limit)
{
    /* Initial allocation if needed */
    if (tri_cap == 0) {
        tri_cap = 128;
        triangular = (seq_t *)malloc(tri_cap * sizeof(seq_t));
    }

    /* Generate next triangular values until exceeding limit */
    while (true) {
        size_t next_k = tri_len + 1;                          /* k = tri_len+1 */
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1) / 2;   /* T_k       */

        if (t > limit) break;                                /* done          */

        /* Grow the array if necessary */
        if (tri_len == tri_cap) {
            tri_cap *= 2;
            triangular = (seq_t *)realloc(triangular, tri_cap * sizeof(seq_t));
        }

        triangular[tri_len++] = t;                           /* store T_k     */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the special case n = 0 */
    if (n == 0) return 0;

    /* Ensure triangular numbers up to n exist in the cache */
    ensure_triangular(n);

    /* Scan the triangular numbers from largest to smallest to find
       the largest divisor.  Since T_1 = 1 always divides n,
       this loop will always terminate. */
    idx_t value = (idx_t) n;          /* temporary integer copy of n */
    seq_t nseq = (seq_t) value;        /* value of n in seq_t */

    size_t i = tri_len;                /* start from the end */
    while (i > 0) {
        i--;                           /* now 0 <= i < tri_len */
        /* Triangular number T_k is triangular[i] (since i = k-1). */
        /* Use idx_t cast for the modulus; T_k <= n <= UINT64_MAX. */
        if (value % ((idx_t)triangular[i]) == 0) {
            return nseq / triangular[i];
        }
    }

    /* Fallback: no triangular divisor found (should not happen due to T_1=1). */
    return nseq;
}
