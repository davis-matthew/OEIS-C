/*
 * A202099  Number of (n+2) X 9 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(alpha+E,E) * C(alpha+O,O) * C(beta+E,E) * C(beta+O,O)
 *            where E=5, O=4, alpha=floor((n+3)/2), beta=floor((n+2)/2)
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 49, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following uses a small multiplicative binomial routine that works
   for all supported seq_t types (fixed‑size integers and arbitrary‑precision). */
#include <stdint.h>

static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    /* use symmetry to minimize number of iterations */
    if (k > n - k) k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* result = result * (n - k + i) / i;   all operations in seq_t */
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset */
    if (n < 1) return (seq_t)0;        /* safety – undefined behavior otherwise */

    const idx_t E = 5;                 /* ceil(9/2)   */
    const idx_t O = 4;                 /* floor(9/2)  */

    idx_t alpha = (n + 3) / 2;          /* floor((n+3)/2) */
    idx_t beta  = (n + 2) / 2;          /* floor((n+2)/2) */

    seq_t term1 = binomial_multiplicative(alpha + E, E);
    seq_t term2 = binomial_multiplicative(alpha + O, O);
    seq_t term3 = binomial_multiplicative(beta  + E, E);
    seq_t term4 = binomial_multiplicative(beta  + O, O);

    return term1 * term2 * term3 * term4;
}
