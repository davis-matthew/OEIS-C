/*
 * A028427  Clog sequence in base 6. Right to left concatenation of
 *          n,int(log_6(n)),int(log_6(int(log_6(n)))),... in base6.
 * https://oeis.org/A028427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value of the base‑6 number obtained by concatenating *
 *            the numbers n, floor(log₆ n), floor(log₆(floor(log₆ n))), … in
 *            base‑6, * with the fragments appended from right to left. The
 *            result is * interpreted as a base‑6 integer and then converted
 *            to decimal
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_log6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log6.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* out of range; just return 0 */

    /* Build the chain n, floor(log₆ n), floor(log₆(...)), 0 */
    idx_t parts[64];
    idx_t cnt = 0;
    idx_t m = n;
    while (m > 0) {
        parts[cnt++] = m;
        m = floor_log6(m);
    }
    parts[cnt++] = 0;                     /* final zero */

    /* Concatenate the parts from right to left */
    seq_t result = 0;
    for (idx_t i = 0; i < cnt; ++i) {
        idx_t v   = parts[cnt - 1 - i];  /* start with the trailing zero */
        idx_t len = (v == 0) ? 1 : floor_log6(v) + 1;  /* digits of v in base‑6 */

        /* 6ⁿ computed by repeated multiplication (len ≤ ~30 for 64‑bit n) */
        seq_t mult = 1;
        for (idx_t j = 0; j < len; ++j) mult *= (seq_t)6;

        result = result * mult + (seq_t)v;
    }
    return result;
}
