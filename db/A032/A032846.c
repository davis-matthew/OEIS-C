/*
 * A032846  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has d(m)
 *          < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th natural number whose base‑8 digits satisfy d(m)
 *            < d(m‑1) > d(m‑2) < … (starting from the most significant digit)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* test whether a value has the required alternating property in base 8 */
static bool alt_octal_ok(seq_t x)
{
    if (x == 0)
        return false;                /* 0 is not in the sequence */

    /* store octal digits, least‑significant first */
    unsigned char digits[128];        /* 128 is enough for 128‑bit seq_t */
    size_t len = 0;

    while (x > 0) {
        digits[len++] = (unsigned char)(x % 8);
        x /= 8;
    }

    /* 1‑digit numbers always satisfy the condition */
    if (len == 1)
        return true;

    /* compare from most significant toward least, alternating "<" and ">" */
    /* for i from len‑1 down to 1 compare digits[i] (higher) and digits[i‑1] */
    for (size_t i = len - 1; i > 0; --i) {
        size_t pos = (len - 1 - i);           /* 0 for first comparison */
        unsigned a = digits[i];
        unsigned b = digits[i - 1];

        if ((pos & 1) == 0) { /* even position: "<" */
            if (!(a < b))
                return false;
        } else {              /* odd position: ">" */
            if (!(a > b))
                return false;
        }
    }

    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 0;

    while (count < n) {
        ++val;                                 /* next integer to test */
        if (alt_octal_ok(val))
            ++count;
    }

    return val;
}
