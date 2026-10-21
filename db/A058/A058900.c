/*
 * A058900  Inconsummate numbers in base 4: no number is this multiple of the
 *          sum of its digits (in base 4).
 * https://oeis.org/A058900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 8392, u32 n <= 8392, u64 n <= 8392,
 *            u128 n <= 8392, big any n
 * Helpers:   sum4digits_and_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A058900 – Inconsummate numbers in base 4  
 *  a(n) is the nth positive integer that never occurs as m / s4(m)
 *  for any integer m, where s4(m) is the sum of the digits of m
 *  written in base 4.  The implementation tests each candidate k
 *  by looking for an s such that s = sum4digits(k*s); if such an
 *  s exists, k is forbidden, otherwise it is part of the sequence. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum4digits_and_digits.h"

/* return the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */

    idx_t k      = 1;                       /* candidate k  */
    idx_t count  = 0;                       /* number of terms found */
    const idx_t maxIdx = ~(idx_t)0;         /* maximum value of idx_t */

    while (1) {
        bool forbidden = false;
        idx_t s = 1;                         /* try all possible sums */

        /* termination guard: avoid overflow of k*s */
        idx_t maxS = (k == 0) ? (maxIdx) : (maxIdx / k);

        while (s <= maxS) {
            idx_t m = k * s;

            int sum = 0, digits = 0;
            sum = sum4digits_and_digits(m, &digits);

            if (sum == (int)s) {            /* found a representation */
                forbidden = true;
                break;
            }
            /* if s exceeds the maximum possible digit sum for m,
               further s cannot match. 3*digits is the maximum sum. */
            if (s > (idx_t)(3 * digits)) break;

            ++s;
        }

        if (!forbidden) {                    /* k is a term of the sequence */
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
