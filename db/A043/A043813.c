/*
 * A043813  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 7 mod 9.
 * https://oeis.org/A043813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A043813: numbers whose base‑3 representation has
   a number of runs congruent to 7 (mod 9).  The function is
   evaluated by brute‑force search, which is fast enough for the
   required range.  */
#include <stdlib.h>
#include <stdbool.h>

/* Count the number of runs in the base‑3 representation of x. */
static int runs_in_base3(seq_t x)
{
    if (x == 0) return 1;          /* “0” in base 3 has one run */
    int runs = 0;
    int last = -1;
    seq_t t = x;
    while (t != 0) {
        int digit = (int)(t % 3);
        if (digit != last) {
            ++runs;
            last = digit;
        }
        t = t / 3;
    }
    return runs;
}

/* True iff x satisfies the sequence condition. */
static inline bool matches(seq_t x)
{
    return (runs_in_base3(x) % 9) == 7;
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    seq_t val = 0;                 /* current number being tested */
    idx_t  found = 0;              /* how many qualifying numbers found */

    while (found < n) {
        ++val;                     /* examine next integer */
        if (matches(val)) {
            ++found;               /* count a new term */
        }
    }
    return val;                    /* val is the n‑th qualifying number */
}
