/*
 * A032898  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i, d(m)
 *          > 0, has d(0) >= d(1) <= d(2) >= ...
 * https://oeis.org/A032898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose decimal digits d(0)…d(m) (with
 *            d(m)>0) satisfy the alternating inequalities d(0) >= d(1) <=
 *            d(2) >= d(3) <= … (starting with “>=”)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether the decimal representation of x satisfies
   d(0) >= d(1) <= d(2) >= d(3) ... */
static bool valid_digits(seq_t x)
{
    seq_t prev = x % 10;   /* d(0) */
    x /= 10;               /* shift right */
    int idx = 0;           /* position of prev (starting at 0) */

    while (x > 0) {
        seq_t curr = x % 10;   /* next digit d(idx+1) */
        if (idx % 2 == 0) {    /* need prev >= curr */
            if (prev < curr) return false;
        } else {               /* need prev <= curr */
            if (prev > curr) return false;
        }
        prev = curr;
        x /= 10;
        ++idx;
    }
    return true;
}

/* Return the nth term of A032898 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but safe return */

    idx_t target = (idx_t)n;
    idx_t count  = 0;
    seq_t x = 1;

    for (;;) {
        if (valid_digits(x)) {
            ++count;
            if (count == target) return x;
        }
        ++x;
    }
}
