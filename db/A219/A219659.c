/*
 * A219659  Irregular table where row n (n >= 0) starts with n, the next term
 *          is A219651(n), and the successive terms are obtained by repeatedly
 *          subtracting the sum of digits in the previous term's factorial
 *          expansion, until zero is reached, after which the next row starts
 *          with one larger n.
 * https://oeis.org/A219659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2097, u16 n <= 2097, u32 n <= 2097, u64 n <= 2097,
 *            u128 n <= 2097, big any n
 * Helpers:   digit_sum_factoradic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A219659 – Irregular table.
 *  For each n ≥ 0 the row starts with n and the next term is
 *  n – digit sum of n in factorial base.
 *  Subsequent terms are obtained by repeatedly subtracting the
 *  factorial‑base digit sum until zero is reached.
 *  a(n) is the n‑th element (0‑based) of the concatenation of all rows.  */

#include <stdbool.h>
#include "../utils/digit_sum_factoradic.h"

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    idx_t idx = 0;          /* current position in the concatenated sequence */
    idx_t row = 0;          /* current row number */

    for (;;) {
        if (row == 0) {                     /* row 0 contains only 0 */
            if (idx == n) return (seq_t)0;
            idx++;
            row++;
            continue;
        }

        seq_t val = (seq_t)row;              /* first element of the row */
        if (idx == n) return val;
        idx++;

        seq_t next = val - digit_sum_factoradic(val);  /* second element */
        if (idx == n) return next;
        idx++;

        while (next > 0) {
            seq_t nxt = next - digit_sum_factoradic(next);
            if (idx == n) return nxt;
            idx++;
            next = nxt;
        }

        row++;
    }
}
