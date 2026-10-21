/*
 * A070246  a(n) = lcm(n, R(n)) / gcd(n, R(n)), where R(n) (A004086) is the
 *          digit reversal of n.
 * https://oeis.org/A070246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 106, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A070246: a(n) = lcm(n, R(n)) / gcd(n, R(n))
 *  where R(n) is the decimal digit reversal of n.
 */
#include <stdint.h>
#include "../utils/gcd_seq.h"

/* Compute the decimal digit reversal of a non‑negative number. */
static seq_t rev(seq_t n)
{
    seq_t rev = 0;
    seq_t tmp = n;
    while (tmp != 0) {
        rev = rev * 10 + (tmp % 10);
        tmp = tmp / 10;
    }
    return rev;
}

/* Greatest common divisor for seq_t values. */

seq_t a(idx_t n)
{
    /* sequence offset is 1; n >= 1 */
    seq_t nSeq = (seq_t)n;
    seq_t r = rev(nSeq);
    seq_t g = gcd_seq(nSeq, r);
    seq_t lcm = (nSeq / g) * r;      /* lcm(n, r) = n/g * r */
    return lcm / g;                  /* a(n) = lcm / gcd */
}
