/*
 * A224760  a(2)=9; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A224760: a(1)=9; a(n)=smallest m such that a(n‑1)+m = concat(a(n‑1), leading digit of m). */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return (seq_t)9;

    seq_t term = (seq_t)9;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        for (int L = 1; L <= 9; ++L) {
            seq_t m = term * 9 + (seq_t)L;   /* candidate m */
            int len = digit_len(m);          /* number of decimal digits of m */
            seq_t pow10len = pow10((idx_t)(len - 1));
            seq_t lead = m / pow10len;       /* leading digit of m */
            if (lead == (seq_t)L) {
                term = m;                    /* a(i) found */
                break;
            }
        }
    }
    return term;
}
