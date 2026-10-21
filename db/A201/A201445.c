/*
 * A201445  Number of n X 2 0..3 arrays with every row and column
 *          nondecreasing rightwards and downwards, and the number of
 *          instances of each value within one of each other.
 * https://oeis.org/A201445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by case-specific polynomial expressions depending
 *            on n mod 4. For n mod 4 = 1: let p = (n+3)/4 → a(n) = 8*p^3 -
 *            2*p^2 For n mod 4 = 2: let q = (n+2)/4 → a(n) = (8*q^3 + 3*q^2 +
 *            q)/6 For n mod 4 = 3: let r = (n+1)/4 → a(n) = 8*r^3 + 10*r^2 +
 *            3*r For n mod 4 = 0: let s = n/4 → a(n) =...
 * Range:     u8 n <= 10, u16 n <= 78, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    switch (n & 3) { /* n % 4 */
    case 1: {
        idx_t p  = (n + 3) / 4;
        seq_t sp = (seq_t)p;
        seq_t sp2 = sp * sp;
        seq_t sp3 = sp2 * sp;
        return 8 * sp3 - 2 * sp2;
    }
    case 2: {
        idx_t q  = (n + 2) / 4;
        seq_t sq = (seq_t)q;
        seq_t sq2 = sq * sq;
        seq_t sq3 = sq2 * sq;
        return (8 * sq3 + 3 * sq2 + sq) / 6;
    }
    case 3: {
        idx_t r  = (n + 1) / 4;
        seq_t sr = (seq_t)r;
        seq_t sr2 = sr * sr;
        seq_t sr3 = sr2 * sr;
        return 8 * sr3 + 10 * sr2 + 3 * sr;
    }
    case 0: {
        idx_t s  = n / 4;
        seq_t ss = (seq_t)s;
        seq_t ss2 = ss * ss;
        seq_t ss3 = ss2 * ss;
        return (8 * ss3 + 21 * ss2 + 19 * ss) / 6 + 1;
    }
    }
    return 0; /* unreachable for n ≥ 1 */
}
