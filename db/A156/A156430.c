/*
 * A156430  Number of n X n arrays of squares of integers, symmetric about
 *          both diagonal and antidiagonal, with all rows summing to 2.
 * https://oeis.org/A156430
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156430:  number of n×n arrays of squares of integers,
   symmetric about both diagonal and antidiagonal, with all rows
   summing to 2.  The sequence satisfies the recurrences

   a(2) = 1,      a(4) = 10,     a(6) = 84, …
   b(0) = 0,      b(2) = 2,      b(4) = 6, …
   c(0) = 0,      c(2) = 1,      c(6) = 6, …

   For k ≥ 3 (i.e. n = 2·k):

       a_even[k] = a_even[k-1]
                 + (k-1)*a_even[k-2]
                 + 4*(k-1)*b_even[k-1]
                 + 2*(k-1)*(k-2)*c_even[k-1];

       b_even[k] = 2*a_even[k-1]
                 + 2*(k-1)*b_even[k-1];

       c_even[k] = 4*b_even[k-1]
                 - 2*a_even[k-2]
                 + 4*(k-2)*a_even[k-3]
                 + 4*(k-2)*(k-3)*c_even[k-2];

   For odd n = 2·k+1:

       a(2k+1) = k * b_even[k].
*/

#include <stdlib.h>
#include <stdint.h>

/* static storage for even-indexed values (k = n/2) */
static seq_t *a_even = NULL;
static seq_t *b_even = NULL;
static seq_t *c_even = NULL;
static size_t a_len = 0;   /* number of computed entries (k values) */
static size_t a_cap = 0;   /* allocated capacity */

/* enlarge the arrays so that they contain entries 0 … kmax */
static void ensure_even(idx_t kmax)
{
    size_t desired = (size_t)kmax + 1;
    if (desired <= a_len) return;

    /* increase capacity if necessary */
    if (desired > a_cap) {
#ifdef __cplusplus
        seq_t *new_a = new seq_t[desired];
        seq_t *new_b = new seq_t[desired];
        seq_t *new_c = new seq_t[desired];
        /* copy old data */
        for (size_t i = 0; i < a_len; ++i) {
            new_a[i] = a_even[i];
            new_b[i] = b_even[i];
            new_c[i] = c_even[i];
        }
        /* free old arrays */
        delete [] a_even;
        delete [] b_even;
        delete [] c_even;
        a_even = new_a;
        b_even = new_b;
        c_even = new_c;
#else
        a_even = realloc(a_even, desired * sizeof(seq_t));
        b_even = realloc(b_even, desired * sizeof(seq_t));
        c_even = realloc(c_even, desired * sizeof(seq_t));
        if (!a_even || !b_even || !c_even) abort();
#endif
        a_cap = desired;
    }

    /* initialise missing entries */
    for (idx_t k = (idx_t)a_len; k <= kmax; ++k) {
        if (k == 0) {
            a_even[0] = (seq_t)1;
            b_even[0] = (seq_t)0;
            c_even[0] = (seq_t)0;
        } else if (k == 1) {
            a_even[1] = (seq_t)1;
            b_even[1] = (seq_t)2;
            c_even[1] = (seq_t)1;
        } else if (k == 2) {
            a_even[2] = (seq_t)10;
            b_even[2] = (seq_t)6;
            c_even[2] = (seq_t)6;
        } else {
            /* b_even[k] = 2*a_even[k-1] + 2*(k-1)*b_even[k-1] */
            seq_t b1 = ((seq_t)2) * a_even[k - 1];
            seq_t b2 = ((seq_t)2) * ((seq_t)(k - 1)) * b_even[k - 1];
            b_even[k] = b1 + b2;

            /* c_even[k] = 4*b_even[k-1] - 2*a_even[k-2]
                         + 4*(k-2)*a_even[k-3]
                         + 4*(k-2)*(k-3)*c_even[k-2] */
            seq_t c1 = ((seq_t)4) * b_even[k - 1];
            seq_t c2 = ((seq_t)2) * a_even[k - 2];
            seq_t c3 = ((seq_t)4) * ((seq_t)(k - 2)) * a_even[k - 3];
            seq_t c4 = ((seq_t)4) * ((seq_t)(k - 2)) * ((seq_t)(k - 3)) * c_even[k - 2];
            c_even[k] = c1 - c2 + c3 + c4;

            /* a_even[k] = a_even[k-1]
                         + (k-1)*a_even[k-2]
                         + 4*(k-1)*b_even[k-1]
                         + 2*(k-1)*(k-2)*c_even[k-1] */
            seq_t a1 = a_even[k - 1];
            seq_t a2 = ((seq_t)(k - 1)) * a_even[k - 2];
            seq_t a3 = ((seq_t)4) * ((seq_t)(k - 1)) * b_even[k - 1];
            seq_t a4 = ((seq_t)2) * ((seq_t)(k - 1)) * ((seq_t)(k - 2)) * c_even[k - 1];
            a_even[k] = a1 + a2 + a3 + a4;
        }
    }
    a_len = desired;
}

/* main sequence entry point */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;            /* values below offset undefined */

    if ((n & 1ULL) == 0ULL) {              /* even n = 2*k */
        idx_t k = n >> 1;
        ensure_even(k);
        return a_even[(size_t)k];
    } else {                               /* odd n = 2*k+1 */
        idx_t k = (n - 1) >> 1;
        ensure_even(k);
        return ((seq_t)k) * b_even[(size_t)k];
    }
}
