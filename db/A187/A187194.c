/*
 * A187194  Number of 7-turn rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Σ_{k=0}^{5} coeff[k] * C(n-3-k+8, 8) for n ≥ 3, where
 *            coeff = {2304, 135936, 842496, 1141056, 431136, 27552}; a(n) = 0
 *            for n < 3. This follows from the generating function
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 10, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial coefficient C(m,8) for m ≥ 8 */
static seq_t binom8(idx_t m)
{
    seq_t res = 1;
    /* Compute product (m-7)(m-6)...m / 8! in a single loop, dividing each step */
    for (idx_t i = 1; i <= 8; ++i)
        res = res * (m - 8 + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;        /* offset: a(1)=a(2)=0 */

    static const seq_t coeff[6] = {
        2304,      /* i = 3  */
        135936,    /* i = 4  */
        842496,    /* i = 5  */
        1141056,   /* i = 6  */
        431136,    /* i = 7  */
        27552      /* i = 8  */
    };

    seq_t total = 0;

    /* k = 0 .. 5 corresponds to i = 3 .. 8 */
    for (idx_t k = 0; k < 6; ++k) {
        idx_t i = 3 + k;                         /* current term index */
        if (n < i) break;                       /* early exit if n < i */
        idx_t m = n - i + 8;                     /* argument of C(m,8) */
        total += coeff[k] * binom8(m);
    }

    return total;
}
