/*
 * A043202  Numbers k such that 3 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Search for numbers k where digits 3 and 5 appear adjacent in *
 *            the base‑9 representation of k, but not in k‑1. This implements
 *            the * definition of OEIS sequence A043202 by incremental search
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n==0 is undefined */
        return (seq_t)0;

    seq_t k = (seq_t)1;
    idx_t count = 0;
    bool prev_has = false;   /* whether k-1 contained adjacent 3 and 5 */

    for (;; ++k) {
        seq_t t = k;
        bool has = false;
        bool has_prev = false;
        seq_t prev_digit = (seq_t)0;

        /* Scan digits of t in base‑9, from least significant to most */
        while (t > (seq_t)0) {
            seq_t d = t % (seq_t)9;
            t = t / (seq_t)9;

            if (has_prev &&
                ((prev_digit == (seq_t)5 && d == (seq_t)3) ||
                 (prev_digit == (seq_t)3 && d == (seq_t)5))) {
                has = true;
            }

            prev_digit = d;
            has_prev = true;
        }

        if (!has) {
            prev_has = false;
            continue;
        }

        if (has && !prev_has) {
            ++count;
            if (count == n)
                return k;
        }
        prev_has = has;
    }
}
