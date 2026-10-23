/*
 * A188427  Position of 5^n in A051037 (5-smooth numbers).
 * https://oeis.org/A188427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{m=1..n} Σ_{i=0..⌊m·ln5/ln2⌋} ( ⌊(m·ln5 –
 *            i·ln2)/ln3⌋ + 1 )
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* 'seq_t' and 'idx_t' are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Pre‑compute the natural logarithms.  Constants are rounded
       to maximum precision that a double can hold. */
    const double ln2 = 0.6931471805599453094172321214581765680755L;
    const double ln3 = 1.0986122886681096913952452369225257046477L;
    const double ln5 = 1.6094379124341003746007611618389728029915L;

    /* Small epsilon to guard against rounding errors in the
       floor operations.  The value 1e-12 is tiny compared
       with ln3 (~1.1) and is safe for all n of interest. */
    const double eps = 1e-12;

    seq_t count = (seq_t)1;          /* start with a(0) */

    for (idx_t m = 1; m <= n; ++m)
    {
        double rhs = (double)m * ln5;                /* m·ln5 */
        idx_t i_max = (idx_t)((rhs / ln2) + eps);    /* ⌊m·ln5/ln2⌋ */

        for (idx_t i = 0; i <= i_max; ++i)
        {
            double val = rhs - (double)i * ln2;     /* m·ln5 – i·ln2 */
            idx_t jmax = (idx_t)((val / ln3) + eps);/* ⌊(m·ln5 – i·ln2)/ln3⌋ */
            count += (seq_t)(jmax + 1);            /* j = 0 .. jmax  */
        }
    }

    return count;
}
