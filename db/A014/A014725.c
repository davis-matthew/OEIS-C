/*
 * A014725  Squares of odd elements in Pascal triangle that are not 1.
 * https://oeis.org/A014725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014414(n)^2, where A014414(n) runs through odd binomial
 *            coefficients > 1 in Pascal's triangle, ordered by row and
 *            position
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                /* offset is 1; this case should not occur */

    idx_t seen = 0;              /* number of odd > 1 coefficients seen so far */

    for (idx_t row = 0; ; ++row) {
        if (row <= 1)
            continue;            /* rows 0 and 1 contain no coefficients > 1 */

        seq_t prev = 1;          /* C(row,0) */
        for (idx_t k = 1; k < row; ++k) {
            /* compute C(row,k) from C(row,k-1) */
            seq_t numerator   = ((seq_t)row - (seq_t)k + 1);
            seq_t curr = prev * numerator / (seq_t)k;

            if ((curr & 1) != 0) {    /* odd coefficient                     */
                seen++;
                if (seen == n) {
                    return curr * curr; /* square of the odd coefficient      */
                }
            }
            prev = curr;
        }
    }
}
