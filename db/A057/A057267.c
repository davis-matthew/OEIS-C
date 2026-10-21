/*
 * A057267  Numbers n such that n | 9^n + 8^n + 7^n + 6^n + 5^n + 4^n + 3^n.
 * https://oeis.org/A057267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A057267.
   a(n) is the n-th positive integer m such that
   m divides (3^m + 4^m + 5^m + 6^m + 7^m + 8^m + 9^m).
   The function scans integers in increasing order and counts those
   that satisfy the divisibility condition. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; /* infinite */ ; ++m) {
        seq_t sum = 0;
        /* Accumulate k^m mod m for k = 3..9 (k small). */
        sum += mod_pow((seq_t)3, m, (seq_t)m);
        sum += mod_pow((seq_t)4, m, (seq_t)m);
        sum += mod_pow((seq_t)5, m, (seq_t)m);
        sum += mod_pow((seq_t)6, m, (seq_t)m);
        sum += mod_pow((seq_t)7, m, (seq_t)m);
        sum += mod_pow((seq_t)8, m, (seq_t)m);
        sum += mod_pow((seq_t)9, m, (seq_t)m);

        if ((sum % (seq_t)m) == (seq_t)0) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
    /* The loop is infinite; never reached. */
    return 0;
}
