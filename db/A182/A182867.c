/*
 * A182867  Triangle read by rows: row n gives coefficients in expansion of
 *          Product_{i=1..n} (x - (2i)^2), highest powers first.
 * https://oeis.org/A182867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient of x^{r-p} in \prod_{i=1}^{r} (x-(2i)^2),
 *            where r is the 0‑based row containing position n and p = n -
 *            r(r+1)/2. Equivalently, a(n) = (-1)^p * e_p, e_p = p‑th
 *            elementary symmetric sum of the numbers 4*i^2 (i=1..r)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 32, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find r and p such that n = r(r+1)/2 + p, 0 <= p <= r. */
    idx_t r = 0;
    idx_t cum = 0;               /* sum of lengths of rows 0..r-1 */
    while (1) {
        idx_t len = r + 1;       /* length of current row r */
        if (cum + len > n)
            break;
        cum += len;
        ++r;
    }
    idx_t p = n - cum;           /* position within row r */

    /* Allocate and zero the e array. */
    size_t sz = (size_t)p + 1;
    seq_t *e = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!e) return 0;            /* allocation failure – undefined */
    e[0] = 1;

    /* Update e_k for k=1..p using the numbers 4*i^2. */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t ai = (seq_t)4 * (seq_t)i * (seq_t)i;
        idx_t maxk = (i < p) ? i : p;      /* min(i,p) */
        for (idx_t k = maxk; k > 0; --k) {
            e[(size_t)k] = e[(size_t)k] + ai * e[(size_t)(k - 1)];
        }
    }

    seq_t coeff = e[(size_t)p];
    free(e);

    /* Apply the sign (-1)^p. */
    if (p & 1)
        coeff = -coeff;
    return coeff;
}
