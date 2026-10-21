/*
 * A228017  Numbers m divisible by the sum of any k-subset of digits of m with
 *          k >= 1.
 * https://oeis.org/A228017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   _a228017_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228017: Numbers m such that every non‑empty subset of its decimal
 * digits sums to a divisor of m.  The sequence is finite; all terms
 * are found by exhaustive search over all 1–4 digit positive integers
 * with no zero digit (there are only 7380 candidates).  This function
 * lazily precomputes the sequence and returns the nth term (1‑based). */

#include <stdbool.h>
#include "../utils/_a228017_check.h"

seq_t a(idx_t n)
{
    static seq_t terms[20];
    static idx_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        count = 0;
        /* search all 1–4 digit numbers with no zero digit (1…9999) */
        for (idx_t num = 1; num <= 9999; ++num) {
            seq_t m = (seq_t)num;
            if (_a228017_check(m))
                terms[count++] = m;
        }
        initialized = true;
    }

    if (n == (idx_t)0 || n > count) return (seq_t)0;  /* out of range */
    return terms[n - 1];
}
