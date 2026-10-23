/*
 * A028426  Clog sequence in base 5. Right to left concatenation of
 *          n,int(log_5(n)),int(log_5(int(log_5(n)))),... in base5.
 * https://oeis.org/A028426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = right‑to‑left concatenation in base 5 of n, ⌊log₅(n)⌋,
 *            ⌊log₅(⌊log₅(n)⌋)⌋, …, 0
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digits_base5, floor_log5, power_of_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digits_base5.h"
#include "../utils/floor_log5.h"
#include "../utils/power_of_5.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of sequence domain; harmless */
    /* Build the log chain: n, ⌊log₅(n)⌋, …, 0. */
    idx_t chain[64];
    size_t cnt = 0;
    idx_t tmp = n;
    for (;;) {
        chain[cnt++] = tmp;
        if (tmp == 0)
            break;
        tmp = floor_log5(tmp);
    }
    /* Concatenate from 0 up to n (right‑to‑left). */
    seq_t result = 0;
    for (size_t idx = cnt; idx > 0; --idx) {
        idx_t m = chain[idx - 1];
        size_t d = digits_base5(m);
        result = result * power_of_5(d) + (seq_t)m;
    }
    return result;
}
