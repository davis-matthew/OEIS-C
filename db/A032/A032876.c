/*
 * A032876  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(0)
 *          > d(1) < d(2) > ...
 * https://oeis.org/A032876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose base‑5 digits satisfy
 *            d(0) > d(1) < d(2) > d(3) < d(4) > … (i.e. the digits alternate
 *            in sign starting with a “greater‑than” at the least significant
 *            place.)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a number satisfies the alternating‑digit pattern. */
static bool is_alt5(seq_t x)
{
    seq_t prev = x % (seq_t)5;   /* d(0) */
    x      /= (seq_t)5;
    idx_t i = 0;                 /* position of prev (least significant so far) */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)5;   /* next higher digit d(i+1) */
        if ((i % (idx_t)2 == 0) ? !(prev > d) : !(prev < d))
            return false;
        prev = d;
        x   /= (seq_t)5;
        i++;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur   = (seq_t)1;     /* start from the first positive integer */

    for (;;) {
        if (is_alt5(cur)) {
            count++;
            if (count == n)
                return cur;
        }
        cur++;
    }
}
