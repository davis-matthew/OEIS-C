/*
 * A077312  Smallest k such that the concatenation of natural numbers from
 *          (n+1) to k is a multiple of n.
 * https://oeis.org/A077312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k ≥ n+1 such that the concatenation of numbers
 *            n+1, n+2, …, k is a multiple of n
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, guard */

    seq_t cur = 0;
    seq_t mod_n = (seq_t)n;

    for (idx_t t = n + 1;; ++t) {
        /* number of decimal digits of t */
        idx_t tmp = t;
        int len = 0;
        while (tmp > 0) {
            ++len;
            tmp /= 10;
        }

        /* 10^len mod n */
        seq_t pow10 = 1;
        for (int i = 0; i < len; ++i)
            pow10 = (pow10 * 10) % mod_n;

        cur = (cur * pow10 + (seq_t)(t % n)) % mod_n;
        if (cur == 0)
            return t;
    }
}
