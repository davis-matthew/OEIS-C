/*
 * A140505  Second differences of Jacobsthal sequence A001045, pairs with even
 *          and odd indices swapped.
 * https://oeis.org/A140505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = J(n+3) - 2·J(n+2) + J(n+1) if n is even a(n) = J(n+1) -
 *            2·J(n) + J(n-1) if n is odd J(k) is the Jacobsthal sequence:
 *            J(0)=0, J(1)=1, J(k)=J(k-1)+2·J(k-2)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of Jacobsthal numbers */
    static seq_t *J = NULL;
    static idx_t J_used = 0;     /* highest index computed (0‑based) */
    static idx_t J_cap  = 0;     /* allocated capacity */

    /* Need J up to index n+3 for any n */
    idx_t need = n + 3;

    /* Allocate/expand cache as needed */
    if (J_cap <= need) {
        idx_t new_cap = J_cap ? J_cap << 1 : 4;
        while (new_cap <= need) new_cap <<= 1;
        J = (seq_t *)realloc(J, new_cap * sizeof(seq_t));
        J_cap = new_cap;
    }

    /* First time: initialise J[0] and J[1] */
    if (J_used == 0) {
        J[0] = (seq_t)0;
        if (J_cap > 1) J[1] = (seq_t)1;
        J_used = 1;   /* highest index computed so far */
    }

    /* Compute missing Jacobsthal numbers up to 'need' */
    while (J_used < need) {
        idx_t i = J_used + 1;
        J[i] = J[i-1] + J[i-2] * ((seq_t)2);
        J_used = i;
    }

    /* Return a(n) according to parity */
    if ((n & 1) == 0) {                /* even n */
        return J[n+3] - ((seq_t)2 * J[n+2]) + J[n+1];
    } else {                            /* odd n, n>=1 */
        return J[n+1] - ((seq_t)2 * J[n]) + J[n-1];
    }
}
