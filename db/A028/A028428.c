/*
 * A028428  Clog sequence in base 7. Right to left concatenation of
 *          n,int(log_7(n)),int(log_7(int(log_7(n)))),... in base7.
 * https://oeis.org/A028428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the base‑7 representations of * n, ⌊log₇
 *            n⌋, ⌊log₇ ⌊log₇ n⌋⌋, ... (the final 0 is omitted because * it
 *            does not change the value). Result is returned as a seq_t
 * Range:     u8 n <= 48, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   int_floor_log7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_floor_log7.h"

/* Number of digits of x in base 7 (x > 0). */
static inline int base7_digits(idx_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 7;
    } while (x > 0);
    return d;
}

seq_t a(idx_t n)
{
    /* Build the chain: n, floor(log₇ n), floor(log₇ ⌊log₇ n⌋), ... */
    idx_t levels[64];      /* enough for any 64‑bit n (log₇ 2⁶⁴ < 24). */
    int len = 0;

    idx_t cur = n;
    while (1) {
        levels[len++] = cur;
        idx_t nxt = int_floor_log7(cur);
        if (nxt == 0)
            break;
        cur = nxt;
    }

    /* Concatenate the levels from the largest log down to n. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        idx_t level = levels[i];
        int digs = base7_digits(level);
        seq_t pow = 1;
        for (int j = 0; j < digs; ++j)
            pow *= 7;
        result = result * pow + level;
    }
    return result;
}
