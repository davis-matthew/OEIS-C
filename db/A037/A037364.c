/*
 * A037364  Numbers whose base-6 and base-8 expansions have no digits in
 *          common.
 * https://oeis.org/A037364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 990 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth positive integer whose base‑6 and base‑8
 * expansions have no common digit. */
#include <stdbool.h>

static bool disjoint(seq_t v)
{
    unsigned m6 = 0;
    unsigned m8 = 0;
    seq_t t;

    if (v == 0)
        return false;  /* 0 shares digit 0 in both bases */

    t = v;
    while (t != 0) {
        unsigned d = (unsigned)(t % 6);
        m6 |= 1u << d;
        t /= 6;
    }

    t = v;
    while (t != 0) {
        unsigned d = (unsigned)(t % 8);
        m8 |= 1u << d;
        t /= 8;
    }

    return (m6 & m8) == 0;
}

seq_t a(idx_t n)
{
    idx_t   count = 0;
    seq_t   x = 0;

    while (count < n) {
        ++x;
        if (disjoint(x))
            ++count;
    }
    return x;
}
