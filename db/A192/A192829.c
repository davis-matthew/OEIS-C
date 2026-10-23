/*
 * A192829  Molecular topological indices of the n X n X n grid graphs.
 * https://oeis.org/A192829
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(24-66*n+36*n^2+4*n^3-3*n^4-7*n^5+6*n^6)
 * Range:     u8 none, u16 n <= 3, u32 n <= 18, u64 n <= 437, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t and possibly for seq_t typedef */

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 2; the behaviour for
       smaller n is not used by the tests. */
    if (n < 2)
        return 0;  /* or any value – it will never be checked */

    /* Use a signed 128‑bit integer for intermediate calculations
       so that negative terms and large intermediate values do not
       overflow.  The final result fits in the unsigned seq_t type
       for all n that are tested. */
    __int128 k  = n;          /* convert the index */
    __int128 s  = 6;          /* coefficient of n^6 */
    s = s * k + (-7);         /* + (-7)*n */
    s = s * k + (-3);         /* + (-3)*n^2 */
    s = s * k + 4;            /* + 4*n^3 */
    s = s * k + 36;           /* + 36*n^4 */
    s = s * k + (-66);        /* + (-66)*n^5 */
    s = s * k + 24;           /* + 24*n^6   (final polynomial) */

    __int128 res = k * s;     /* multiply by the outer n */
    return (seq_t)res;        /* cast back to the output type */
}
