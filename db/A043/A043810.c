/*
 * A043810  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 4 mod 9.
 * https://oeis.org/A043810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043810: nth number whose base‑3 representation has a number of runs
   congruent to 4 modulo 9.  This implementation scans integers
   incrementally and counts the required property. */
#include <stdint.h>

/* Count the number of runs in the base‑3 representation of x. */
static int count_runs(seq_t x)
{
    int runs = 0;
    int last = -1; /* sentinel for “no previous digit” */

    while (x != 0) {
        int digit = (int)(x % (seq_t)3);
        if (last == -1 || digit != last)
            ++runs;
        last = digit;
        x /= 3;
    }
    return runs;
}

/* Return the nth term of A043810 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 */

    seq_t current = 0;
    idx_t found   = 0;

    while (found < n) {
        ++current; /* inspect next integer */
        if (count_runs(current) % 9 == 4)
            ++found;
    }
    return current;
}
