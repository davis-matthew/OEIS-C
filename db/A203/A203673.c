/*
 * A203673  Vandermonde sequence using x^2 + xy + y^2 applied to
 *          (1,4,9,...,n^2).
 * https://oeis.org/A203673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤i<j≤n} (i⁴ + i²j² + j⁴)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1 */

    seq_t result = 1;           /* empty product when n = 1 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t i_sq = (seq_t)i * (seq_t)i;          /* i² */
        seq_t i_sq_sq = i_sq * i_sq;              /* i⁴ */

        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t j_sq = (seq_t)j * (seq_t)j;      /* j² */
            seq_t j_sq_sq = j_sq * j_sq;          /* j⁴ */

            seq_t term = i_sq_sq + i_sq * j_sq + j_sq_sq;  /* i⁴ + i²j² + j⁴ */
            result = result * term;
        }
    }

    return result;
}
