/*
 * A062904  Numbers n such that n and its reversal are both multiples of 14.
 * https://oeis.org/A062904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A062904 – Numbers n such that n and its decimal reversal are
   both multiples of 14.  The function returns a(n) for n >= 1.
   It implements a straightforward linear search over the multiples of
   14, checking that each candidate and its reversal are divisible by
   14.  Reversal is performed directly on seq_t to support arbitrary‑
   precision builds. */

#include <stdint.h>
#include <stdbool.h>

/* Reverse the decimal representation of a seq_t value. */
static seq_t reverse_seq_t(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;            /* undefined for offset 1; safety */

    seq_t cur   = 0;                  /* current candidate (multiple of 14) */
    idx_t count = 0;                  /* how many valid terms seen so far */

    while (count < n) {
        cur += (seq_t)14;              /* next multiple of 14 */
        if ((cur % 14) == 0) {         /* redundant but kept for clarity */
            seq_t rev = reverse_seq_t(cur);
            if ((rev % 14) == 0) {
                count++;               /* cur is a valid term */
            }
        }
    }
    return cur;                       /* cur is the nth term */
}
