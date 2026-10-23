/*
 * A129001  Heights of roots in Cartan root systems for exceptional groups:
 *          A1, G2, F4, E6, E7, E8.
 * https://oeis.org/A129001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf, uned
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th coefficient of the concatenation of the
   highest root coefficients for the exceptional root systems
   A₁, G₂, F₄, E₆, E₇, E₈.  The coefficient lists are
   [1],
   [3, 2],
   [2, 3, 4, 2],
   [1, 2, 2, 3, 2, 1],
   [2, 2, 3, 4, 3, 2, 1],
   [2, 3, 4, 6, 5, 4, 3, 2].  */

/* External linkage required by the test harness */
seq_t a(idx_t n)
{
    /* concatenated coefficient list of length 28 */
    static const seq_t coeffs[] =
    {
        1,          /* A1 */
        3, 2,       /* G2 */
        2, 3, 4, 2, /* F4 */
        1, 2, 2, 3, 2, 1,            /* E6 */
        2, 2, 3, 4, 3, 2, 1,         /* E7 */
        2, 3, 4, 6, 5, 4, 3, 2       /* E8 */
    };

    const idx_t len = sizeof(coeffs) / sizeof(coeffs[0]);

    if (n < 1 || n > len)
        return (seq_t)0; /* undefined beyond known terms */

    return coeffs[n - 1];
}
