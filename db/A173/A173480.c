/*
 * A173480  a(n) = the largest n-digit number ending in n-1 zeros that is
 *          divisible by n, else 0.
 * https://oeis.org/A173480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) is the largest n‑digit number ending with n-1 zeros that is
 *            divisible by n; if no such number exists return 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Build 10^(n-1) in the big integer type (powv) and keep its remainder
       modulo n in a 64‑bit variable (rem).  n ≥ 1. */
    seq_t powv = (seq_t)1;
    uint64_t rem = 1 % n;   /* remainder of 10^0 modulo n */

    for (idx_t i = 1; i < n; ++i) {
        powv *= (seq_t)10;          /* multiply by 10 (big‑int) */
        rem = (rem * 10) % n;       /* remainder of 10^i modulo n */
    }

    /* Find the largest digit d in 1..9 such that d*powv is divisible by n.
       Since rem = 10^(n-1) mod n, we just need (rem * d) mod n == 0. */
    idx_t d = 0;
    for (idx_t test = 9; test >= 1; --test) {
        if ((rem * test) % n == 0) {
            d = test;
            break;
        }
    }

    if (d == 0)          /* no suitable digit found */
        return (seq_t)0;

    /* Return the final number: d * 10^(n-1). */
    return powv * (seq_t)d;
}
