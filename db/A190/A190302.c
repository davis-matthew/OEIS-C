/*
 * A190302  Smallest number h such that the decimal expansion of n*h starts
 *          with 1.
 * https://oeis.org/A190302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 777, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A190302 – Smallest h such that the decimal expansion of n*h starts with 1.
 *  a(n) = A187285(n) / n, where A187285(n) is the smallest multiple of n whose decimal
 *  representation begins with the digit 1.  This implementation performs a
 *  straightforward search over successive multipliers h until the leading digit
 *  of n*h equals 1.
 *
 *  The algorithm is O(h·log(n*h)) with h ≤ 5 in all known cases (conjectured to
 *  always hold).  For arbitrary values of n it keeps searching until it finds the
 *  required multiple; the result always fits in seq_t if the corresponding
 *  multiple does.
 */
#include <stdbool.h>
#include "../utils/pow10.h"      /* provides seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"  /* provides int digit_len(seq_t x); */

seq_t a(idx_t n)
{
    /* start with multiplier 1 and increment until the product starts with 1 */
    idx_t h = 1;
    for (;; ++h) {
        seq_t prod = (seq_t)n * (seq_t)h;        /* n * h */
        int     d   = digit_len(prod);           /* number of decimal digits */
        seq_t   pw  = pow10((idx_t)d - 1);        /* 10^(d‑1)  */
        seq_t   lead= prod / pw;                 /* leading digit */
        if (lead == 1)
            return (seq_t)h;                     /* a(n) = h */
    }
}
