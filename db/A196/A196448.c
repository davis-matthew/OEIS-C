/*
 * A196448  Value of a variable (d) in an iterative system. See equations.
 * https://oeis.org/A196448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equation system from OEIS A196448:
 *   a = xx, b = xX, c = XX, d = xY, e = XY.
 *   a' = 2ad + bd;
 *   b' = 2ae + bd + be + 2cd;
 *   c' = be + 2ce;
 *   d' = bd + be + 2ad + 2ae;
 *   e' = bd + be + 2cd + 2ce.
 *
 * Starting from F₀ : a=0, b=1, c=0, d=0, e=1,
 * a(n) is the value of d after n successive iterations.
 *
 * Initial values: a(0)=0, a(1)=1, a(2)=2,...
 * ---------------------------------------------------------------------------------
 */

seq_t a(idx_t n)
{
    /* Initial genotype counts for generation 0 */
    seq_t aV = 0, bV = 1, cV = 0, dV = 0, eV = 1;

    /* Iterate n generations */
    for (idx_t i = 0; i < n; ++i) {
        seq_t na = 2 * aV * dV + bV * dV;
        seq_t nb = 2 * aV * eV + bV * dV + bV * eV + 2 * cV * dV;
        seq_t nc = bV * eV + 2 * cV * eV;
        seq_t nd = bV * dV + bV * eV + 2 * aV * dV + 2 * aV * eV;
        seq_t ne = bV * dV + bV * eV + 2 * cV * dV + 2 * cV * eV;

        aV = na; bV = nb; cV = nc; dV = nd; eV = ne;
    }

    return dV;   /* a(n) = value of d after n iterations */
}
