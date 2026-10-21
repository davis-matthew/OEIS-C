/*
 * A203755  Vandermonde sequence using x^4 + y^4 applied to
 *          (0,1,1,2,2,...,floor(n/2)).
 * https://oeis.org/A203755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤i<j≤n} (⌊i/2⌋^4 + ⌊j/2⌋^4) (OEIS A203755)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, idx_t is defined elsewhere */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;  /* a(1) = 1, and a(0) is undefined but we return 1 for safety */

    seq_t result = 1;

    for (idx_t i = 1; i < n; ++i) {
        seq_t fi = (seq_t)(i / 2);          /* ⌊i/2⌋ */
        seq_t fi2 = fi * fi;                /* (⌊i/2⌋)^2 */
        seq_t fi4 = fi2 * fi2;              /* (⌊i/2⌋)^4 */

        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t fj = (seq_t)(j / 2);      /* ⌊j/2⌋ */
            seq_t fj2 = fj * fj;            /* (⌊j/2⌋)^2 */
            seq_t fj4 = fj2 * fj2;          /* (⌊j/2⌋)^4 */

            result *= (fi4 + fj4);
        }
    }

    return result;
}
