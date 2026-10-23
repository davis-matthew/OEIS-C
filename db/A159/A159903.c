/*
 * A159903  Concatenation of the first n nonsquarefree numbers A013929.
 * https://oeis.org/A159903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = decimal concatenation of the first n nonsquarefree
 *            numbers. The sequence A159903 is defined as a(n) =
 *            concat(A013929(1), …, A013929(n)), where A013929 lists the
 *            nonsquarefree integers. This implementation generates the
 *            nonsquarefree integers on the fly, concatenating their decimal
 *            represen...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   digit_len, is_squarefree, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;               /* offset starts at 1; 0 is unused */
    }

    idx_t count = 0;            /* how many nonsquarefree numbers seen */
    idx_t cand  = 0;            /* current integer being tested */
    seq_t result = 0;           /* current concatenated value */
    bool first = true;          /* whether this is the first nonsquarefree */

    for (;;) {
        ++cand;
        if (!is_squarefree((seq_t)cand)) {
            ++count;
            if (first) {
                result = (seq_t)cand;
                first = false;
            } else {
                int d = digit_len((seq_t)cand);      /* number of decimal digits of cand */
                seq_t ten_pow = pow10((idx_t)d);     /* 10^d as seq_t */
                result = result * ten_pow + (seq_t)cand;
            }
            if (count == n) {
                return result;
            }
        }
    }
}
