/*
 * A067632  n(n+180) is a square.
 * https://oeis.org/A067632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 15, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067632: all integer n such that n*(n+180) is a perfect square.
   The solutions are found by solving (n+90)^2 - k^2 = 90^2, which reduces to
   (n+90 - k)(n+90 + k) = 8100 = 4 * 2025.  Thus set A = 2a, B = 2b with ab = 2025.
   For each divisor pair (a,b) we obtain two solutions:
        n = a + b - 90          (A,B positive)
        n = -(a + b) - 90      (A,B negative)
   The sequence is finite and consists of the 16 values listed in the OEIS
   entry.  This routine generates those ten solutions programmatically and
   returns the n‑th element in ascending order. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence has exactly 16 terms.  Compute them once, cache, and
       return the requested index. */
    static seq_t values[16];
    static bool initialized = false;

    if (!initialized) {
        const idx_t limit = 2025;          /* product ab = 2025   */
        idx_t count = 0;

        /* Iterate over all divisor pairs (a,b) with a <= b. */
        for (idx_t a = 1; a * a <= limit; ++a) {
            if (limit % a == 0) {
                idx_t b = limit / a;
                seq_t sum = (seq_t)a + (seq_t)b;

                /* Positive solution (A,B > 0). */
                values[count++] = sum - (seq_t)90;

                /* Negative solution (A,B < 0). */
                values[count++] = -(sum) - (seq_t)90;
            }
        }

        /* count should be 16; sort ascending. */
        for (idx_t i = 1; i < 16; ++i) {
            seq_t key = values[i];
            idx_t j = i;
            while (j > 0 && values[j - 1] > key) {
                values[j] = values[j - 1];
                --j;
            }
            values[j] = key;
        }
        initialized = true;
    }

    if (n < 16)
        return values[n];

    /* For indices beyond the known terms the sequence is undefined. */
    return (seq_t)0;
}
