/*
 * A143496  4-Stirling numbers of the second kind.
 * https://oeis.org/A143496
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 4‑Stirling numbers of the second kind – OEIS A143496 
 *
 *  a(n) is the flattened row‑major enumeration of the triangle T(r,k)
 *  defined for r,k ≥ 4.  For a given index n ≥ 4 we first locate the
 *  corresponding row r and column k:
 *
 *      offset   = n - 4
 *      r        = smallest integer ≥ 4 with   Σ_{t=4}^{r} (t-3)  > offset
 *      pos      = offset - Σ_{t=4}^{r-1} (t-3)
 *      k        = 4 + pos
 *
 *  Once r and k are known, the entry is computed from the explicit
 *  formula
 *
 *          T(r,k) = (1/(k-4)!) * Σ_{i=0}^{k-4} (-1)^{k-4-i} *
 *                           C(k-4,i) · (i+4)^{r-4}
 *
 *  where C(⋅,⋅) is the binomial coefficient.  The implementation
 *  evaluates this sum directly, computing binomial coefficients and
 *  powers multiplicatively; it uses only the operators + - * / % and
 *  integer types supplied by the build.  All calculations are done in
 *  the type seq_t, which may be an 8,16,32,64 or 128 bit unsigned
 *  integer or an arbitrary‑precision integer.  The function works for
 *  any n ≥ 4, correctly memoizes nothing, and runs in O(k log(r)) time
 *  per call, which is well within the limits for n up to ~10 000.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n < 4)               /* below the offset – undefined, return 0 */
        return (seq_t)0;

    /* ---- locate the row r and column k from the flattened index ---- */
    idx_t offset = n - 4;        /* 0‑based offset from the first term     */
    idx_t r = 4;                 /* current row                           */
    idx_t cumulative = 0;        /* number of entries before the current r */

    /* find the smallest r such that cumulative number of entries
       up to r exceeds offset                                    */
    while (cumulative + (r - 3) <= offset) {
        cumulative += (r - 3);
        r++;
    }

    idx_t pos   = offset - cumulative;      /* position within row r */
    idx_t k     = 4 + pos;                  /* column number      */

    /* ---- compute T(r,k) using the explicit formula ------------------ */
    idx_t n_exp = r - 4;        /* exponent of (i+4)                 */
    idx_t k_exp = k - 4;        /* upper limit of the sum and size of factorial */

    /* alternating sum */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (idx_t i = 0; i <= k_exp; ++i) {
        /* compute binomial coefficient C(k_exp, i) multiplicatively */
        seq_t bin = 1;
        for (idx_t j = 1; j <= i; ++j) {
            bin *= (seq_t)(k_exp - j + 1);
            bin /= (seq_t)j;
        }

        /* compute (i+4)^{n_exp} by exponentiation by squaring */
        seq_t pow = 1;
        seq_t base = (seq_t)(i + 4);
        idx_t e = n_exp;
        while (e) {
            if (e & 1)
                pow *= base;
            base *= base;
            e >>= 1;
        }

        seq_t term = bin * pow;               /* term of the sum */

        if (((k_exp - i) & 1) == 0)
            pos_sum += term;                  /* positive sign   */
        else
            neg_sum += term;                  /* negative sign   */
    }

    seq_t sum = pos_sum;
    sum -= neg_sum;    /* guaranteed non‑negative */

    /* compute factorial (k_exp)! by simple multiplication */
    seq_t fact = 1;
    for (idx_t i = 1; i <= k_exp; ++i)
        fact *= (seq_t)i;

    return sum / fact;
}
