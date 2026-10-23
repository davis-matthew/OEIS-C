/*
 * A043797  Numbers whose number of runs in base 3 is congruent to 6 (mod 7).
 * https://oeis.org/A043797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose number of runs * in its
 *            base‑3 representation is congruent to 6 modulo 7
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the runs of consecutive equal digits in the base‑3 representation
 * of x.  x may be zero; for zero there is a single run consisting of the
 * digit 0. */
static unsigned int runs_in_base3(seq_t x)
{
    if (x == 0)
        return 1;

    unsigned int runs = 1;
    seq_t prev = x % 3;
    x /= 3;
    while (x != 0) {
        seq_t cur = x % 3;
        if (cur != prev) {
            ++runs;
            prev = cur;
        }
        x /= 3;
    }
    return runs;
}

/* Return the n-th element of the sequence (1‑based index).
 * For n == 0 the function returns 0 – the value is undefined in the
 * OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t found = 0;        /* how many qualifying numbers we have seen */
    seq_t x = 0;           /* current integer to test (starts at 0) */

    while (found < n) {
        ++x;                        /* check next integer */
        unsigned int r = runs_in_base3(x);
        if ((r % 7) == 6)
            ++found;
    }

    return x;
}
