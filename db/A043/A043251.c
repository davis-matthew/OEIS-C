/*
 * A043251  Numbers k such that 5 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer k for which the decimal
 *            representation of k contains the adjacent digits “56” or “65”,
 *            while the representation of k‑1 does not
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* test whether x contains “56” or “65” as consecutive digits */
static bool has56or65(idx_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = x % 10;
        if ((prev == 5 && d == 6) || (prev == 6 && d == 5))
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* behaviour for the offset is irrelevant */

    idx_t count = 0;
    idx_t num   = 56;      /* the smallest eligible number */

    for (;;) {
        if (has56or65(num) && !has56or65(num - 1)) {
            count++;
            if (count == n)
                return (seq_t)num;
        }
        num++;
    }
}
