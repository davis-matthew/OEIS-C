/*
 * A059708  Numbers k such that all digits have same parity.
 * https://oeis.org/A059708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = the n-th non‑negative integer whose decimal digits all
 *            share the same parity (all even or all odd). a(1) = 0, a(2) = 1,
 *            a(3) = 2, …
 * Range:     u8 n <= 95, u16 n <= 4905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool same_parity(seq_t x)
{
    /* Treat 0 as even.  The following loop automatically handles it. */
    int parity = 0;            /* initialized to avoid unused variable warning */
    if (x > 0)                 /* non‑zero numbers: determine parity of a digit */
        parity = (int)((x % 10) & 1);

    while (x > 0) {
        int d = (int)((x % 10) & 1);
        if (d != parity)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t val = 0;; ++val) {      /* val is seq_t, works for any size */
        if (same_parity(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
