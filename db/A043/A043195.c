/*
 * A043195  Numbers k such that 2 and 3 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: nth number k whose base‑9 representation contains either
   the adjacent digits 2 and 3 in any order (23 or 32), but its
   predecessor k-1 does not. */
#include <stdbool.h>
#include <stdint.h>

/* Return true iff the base‑9 representation of x contains "23" or
   "32" as adjacent digits. */
static bool has23or32(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    /* 128‑bit numbers need at most 64 base‑9 digits; array size is safe. */
    uint8_t d[64];
    int len = 0;

    while (x != (seq_t)0) {
        d[len++] = (uint8_t)(x % (seq_t)9);
        x /= (seq_t)9;
    }

    /* Scan from most significant digit to least. */
    for (int i = len - 1; i > 0; --i) {
        if ((d[i] == 2 && d[i - 1] == 3) || (d[i] == 3 && d[i - 1] == 2))
            return true;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = (seq_t)1;

    while (true) {
        if (has23or32(k) && !has23or32(k - (seq_t)1))
        {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
