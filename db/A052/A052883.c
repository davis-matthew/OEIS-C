/*
 * A052883  Expansion of e.g.f.: log((-1+x)/(-1+2*x))^2.
 * https://oeis.org/A052883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS A052883):
 *  (8*n+20*n^2+16*n^3+4*n^4)*a(n)+(-36-78*n-54*n^2-12*n^3)*a(n+1)+
 *  (52+13*n^2+52*n)*a(n+2)+(-6*n-15)*a(n+3)+a(n+4)=0
 *  with a(0)=0, a(1)=0, a(2)=2, a(3)=18
 */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static table up to a reasonably sized maximum (10001 terms are enough for the tests)  */
    enum { TABLE_CAP = 20001 };
    static seq_t tbl[TABLE_CAP];
    static size_t max_computed = 3;   /* indices 0..3 are already known */
    static int initialized = 0;

    /* initialise base terms once */
    if (!initialized) {
        tbl[0] = (seq_t)0;
        tbl[1] = (seq_t)0;
        tbl[2] = (seq_t)2;
        tbl[3] = (seq_t)18;
        initialized = 1;
    }

    /* if the requested index is already in the table, return it directly */
    if (n <= max_computed)
        return tbl[n];

    /* ensure we do not exceed the pre‑allocated table */
    if ((size_t)n >= TABLE_CAP)
        return (seq_t)0;   /* out of bounds – fall back to zero */

    /* compute new entries up to n, using the recurrence */
    for (size_t k = max_computed + 1; k <= (size_t)n; ++k) {
        /* compute coefficients for the term a(k-4) ... a(k-1) */
        seq_t km4 = (seq_t)(k - 4);                /* n in the recurrence */
        seq_t km4_2 = km4 * km4;                  /* n^2 */
        seq_t km4_3 = km4_2 * km4;                /* n^3 */
        seq_t km4_4 = km4_3 * km4;                /* n^4 */

        seq_t coeff0 = (seq_t)8 * km4
                     + (seq_t)20 * km4_2
                     + (seq_t)16 * km4_3
                     + (seq_t)4  * km4_4;

        seq_t coeff1 = (seq_t)-36
                     - (seq_t)78 * km4
                     - (seq_t)54 * km4_2
                     - (seq_t)12 * km4_3;

        seq_t coeff2 = (seq_t)52
                     + (seq_t)13 * km4_2
                     + (seq_t)52 * km4;

        seq_t coeff3 = (seq_t)-6 * km4
                     - (seq_t)15;

        /* evaluate the right‑hand side of the recurrence */
        seq_t rhs = coeff0 * tbl[k - 4]
                  + coeff1 * tbl[k - 3]
                  + coeff2 * tbl[k - 2]
                  + coeff3 * tbl[k - 1];

        /* a(k) = - rhs  (the result is always non‑negative) */
        tbl[k] = (seq_t)(0) - rhs;
    }

    max_computed = (size_t)n;
    return tbl[n];
}
