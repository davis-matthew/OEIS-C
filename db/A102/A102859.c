/*
 * A102859  Numbers that when squared and written backwards give a square
 *          again.
 * https://oeis.org/A102859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_square, rev_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = sqrt(A061457(n))
 *   a(n) is the nth non‑negative integer k such that k² reversed is a perfect
 *   square.  The function scans integers in increasing order and tests the
 *   property on each.
 */

#include <stdbool.h>
#include "../utils/is_square.h"
#include "../utils/rev_of.h"

/* return the n‑th term of A102859, n >= 1 */
seq_t a(idx_t n) {
    if (n == 0) {
        /* the sequence is defined only for n >= 1; return 0 as a safe
           fallback for an illegal call */
        return (seq_t)0;
    }

    idx_t count = 0;
    idx_t m_idx = 0;

    while (1) {
        seq_t m = (seq_t)m_idx;
        seq_t sq = m * m;
        seq_t rev_sq = rev_of(sq);

        if (is_square(rev_sq)) {
            ++count;
            if (count == n) {
                return m;
            }
        }
        ++m_idx;
    }
}
