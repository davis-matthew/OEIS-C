/*
 * A214777  McNugget numbers: numbers of the form 6*x + 9*y + 20*z for
 *          nonnegative integers x, y, z.
 * https://oeis.org/A214777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+21 for n >= 23; * For n ≤ 22, a(n) is the nth smallest
 *            non‑negative integer * not among the set
 *            {1,2,3,4,5,7,8,10,11,13,14,16,17,19,22,23,25,28,31,34,37,43}
 * Range:     u8 n <= 234, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const idx_t missing[] = {
    1,2,3,4,5,7,8,10,11,13,14,16,17,19,22,23,25,28,31,34,37,43
};
static const size_t missingcnt = sizeof(missing)/sizeof(missing[0]);

seq_t a(idx_t n)
{
    /* Handle the special case for small n (1..22). */
    if (n <= missingcnt) {
        idx_t count = 0;
        for (idx_t x = 0; ; ++x) {
            int is_missing = 0;
            for (size_t i = 0; i < missingcnt; ++i) {
                if (x == missing[i]) {
                    is_missing = 1;
                    break;
                }
            }
            if (!is_missing) {
                ++count;
                if (count == n)
                    return (seq_t)x;
            }
        }
    }
    /* For n ≥ 23, all numbers ≥ 44 are representable. */
    return (seq_t)(n + 21);
}
