/*
 * A030141  Numbers in which parity of the decimal digits alternates.
 * https://oeis.org/A030141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth non‑negative integer whose decimal digits *
 *            alternate in parity (the first few terms are 0, 1, 2, 3, 4, 5,
 *            6, 7, * 8, 9, 10, 12, 14, …). This implementation finds the
 *            value by * brute‑force search, which is fast enough for n up to
 *            about 10 000
 * Range:     u8 n <= 93, u16 n <= 4855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: returns true if decimal digits of x alternate in parity. */
static bool is_alternating(seq_t x)
{
    if (x == 0) return true;          /* 0 is considered alternating */

    int prev_parity = -1;
    bool first = true;
    while (x > 0) {
        int d = (int)(x % 10);
        int p = d & 1;
        if (!first && p == prev_parity) return false;
        prev_parity = p;
        first = false;
        x /= 10;
    }
    return true;
}

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t num = 0;

    for (;;) {
        if (is_alternating(num)) {
            count++;
            if (count == (seq_t)n)
                return num;
        }
        num++;
    }
}
