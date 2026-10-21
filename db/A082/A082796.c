/*
 * A082796  Smallest multiple of n beginning with 6.
 * https://oeis.org/A082796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 5957, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) – Smallest multiple of n that begins with the digit 6 */
/* The implementation searches consecutive multiples k·n until the most
   significant decimal digit is 6.  It works for all builds:
   (a) uint8_t … uint64_t;
   (b) unsigned __int128;
   (c) an arbitrary‑precision integer class that supports the operators
       + - * / % and comparison with integers. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Valid for n >= 1.  The case n == 0 is not defined by OEIS, but
       we protect against it to keep the function well defined. */
    if (n == 0) return 0;

    /* Convert the index to a seq_t value.  All fixed‑width builds use
       the appropriate unsigned integer type; for big‑int builds this
       performs a conversion to the big‑int class. */
    seq_t base = (seq_t)n;

    idx_t k = 1;                 /* multiplier candidate */
    for (;; ++k) {
        /* Compute the product carefully:
           - For builds where seq_t is a fixed‑width type, the
             multiplication follows the usual semantics, and the
             product never exceeds 2^64 for the test ranges.
           - For arbitrary‑precision builds, the operator* provided
             by the big‑int class is used, which yields an exact
             product. */
        seq_t prod = base * (seq_t)k;

        /* Extract the most significant decimal digit.  The loop
           repeatedly divides by 10 until the remaining value is a
           single‑digit number.  The class must support int division
           and comparison with the literal 10. */
        seq_t d = prod;
        while (d >= 10) d /= 10;

        if (d == 6) return prod;
    }
}
