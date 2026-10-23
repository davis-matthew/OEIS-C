/*
 * A163522  a(1)=2; for n>1, a(n) = sum of digits of a(n-1)^2.
 * https://oeis.org/A163522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   digit_sum, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163522: a(1)=2; for n>1, a(n) = sum_of_digits( a(n-1)^2 ) */

/* The implementation is intentionally straightforward: it iterates from
   the start of the sequence up to the requested index n, computing each
   term via a cheap square and a digit‑sum.  It uses the generic utility
   functions `seq_pow` and `digit_sum`, which work for all supported
   integer types (fixed‑width or arbitrary precision). */

#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/digit_sum.h"  /* digit_sum(seq_t) */

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n<1 – return 0 as a safe default */
        return (seq_t)0;

    seq_t cur = (seq_t)2;       /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = sum_of_digits( a(i-1)^2 ) */
        seq_t square = seq_pow(cur, (idx_t)2);
        cur = digit_sum(square);
    }

    return cur;
}
