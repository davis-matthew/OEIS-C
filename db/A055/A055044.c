/*
 * A055044  Numbers of the form 2^(2i+1)*(8*j+1).
 * https://oeis.org/A055044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9603 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of numbers of the form 2^(2i+1)*(8*j+1) (i,j ≥ 0),
   ordered increasingly.  This implementation scans natural numbers
   and returns the nth term. */
#include <stdbool.h>

static bool term_ok(seq_t x)
{
    seq_t y = x;
    uint64_t parity = 0;          /* parity of v2(x); 1 if odd, 0 if even */
    while ((y % 2) == 0) {
        y /= 2;
        parity ^= 1;             /* toggle parity */
    }
    if (parity == 0) return false;      /* exponent is even or zero */
    return (y % 8) == 1;                 /* odd part ≡ 1 (mod 8) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* defined only for n ≥ 1 */
    seq_t cur = 2;           /* first candidate */
    idx_t count = 0;

    while (true) {
        if (term_ok(cur)) {
            count++;
            if (count == n) return cur;
        }
        cur += 1;
    }
}
