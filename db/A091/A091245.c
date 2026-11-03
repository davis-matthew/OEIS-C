/*
 * A091245  Number of reducible GF(2)[X]-polynomials in range [0,n].
 * https://oeis.org/A091245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of GF(2)[X] polynomials in the range [0,n] that
 *            are reducible (degree ≥ 2 and admit a non‑trivial factor).
 *            Implemented by a straightforward check of each integer up to n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 73 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/*--- Compute the degree of a binary polynomial ---------------------------------*/
static inline int poly_deg(idx_t v) {
    /* Return -1 for 0, 0 for constants 1; otherwise highest set bit position. */
    if (v <= 1) return -1;
    int d = 0;
    while (v >>= 1) d++;
    return d;
}

/*--- Test whether a polynomial represented by an integer is reducible in GF(2) -*/
static inline int is_reducible(idx_t poly) {
    int d = poly_deg(poly);
    if (d < 2) return 0;                /* constants and linear polynomials are irreducible */

    /* Try all possible proper factors of degree <= d/2 */
    for (int f = 1; f <= d / 2; ++f) {
        idx_t base = ((idx_t)1) << f;            /* leading bit must be set          */
        idx_t limit = base << 1;                 /* corresponds to (1<<(f+1))        */

        for (idx_t fac = base; fac < limit; ++fac) {
            idx_t rem = poly;

            /* Polynomial long division in GF(2) */
            while (poly_deg(rem) >= f) {
                int shift = poly_deg(rem) - f;
                rem ^= fac << shift;
            }
            if (rem == 0) return 1;              /* divisible → reducible            */
        }
    }
    return 0;                                  /* no divisor found                  */
}

/*--- Main sequence function -----------------------------------------------------*/
seq_t a(idx_t n) {
    seq_t cumulative = 0;            /* start with zero count */
    for (idx_t i = 0; i <= n; ++i) {
        if (is_reducible(i)) cumulative++;
    }
    return cumulative;
}
