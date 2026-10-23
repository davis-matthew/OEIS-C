/*
 * A010077  a(n) = sum of digits of a(n-1) + sum of digits of a(n-2); a(0) =
 *          0, a(1) = 1.
 * https://oeis.org/A010077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0)=0, a(1)=1, a(2)=1; a(n)=digit_sum(a(n-1)) +
 *            digit_sum(a(n-2)) for n≥3. The sequence is periodic from n=3
 *            with period 24
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */
#include <stdbool.h>       /* for bool */
#include "../utils/digit_sum.h"   /* provides digit_sum(seq_t) */

seq_t a(idx_t n)
{
    /* table[0..26] holds the first 27 terms.  Values 0..2 are the
       given starting values; 3..26 are produced by the recurrence. */
    static seq_t table[27];
    static bool init = false;

    if (!init) {
        table[0] = 0;
        table[1] = 1;
        table[2] = 1;
        for (idx_t i = 3; i <= 26; ++i) {
            table[i] = digit_sum(table[i - 1]) + digit_sum(table[i - 2]);
        }
        init = true;
    }

    if (n <= 26) {
        return table[(size_t)n];
    }
    /* For n >= 27 the subsequence starting at a(3) repeats every 24
       terms.  Use the mapping a(n) = a(3 + ((n-3) mod 24)). */
    size_t idx = 3 + ((size_t)(n - 3) % 24);
    return table[idx];
}
