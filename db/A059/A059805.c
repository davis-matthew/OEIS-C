/*
 * A059805  Natural numbers written with digits grouped in pairs and leading
 *          zeros omitted.
 * https://oeis.org/A059805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) is the n‑th pair of digits obtained by concatenating * the
 *            decimal representations of the natural numbers 1,2,3,…, *
 *            splitting the resulting digit stream into consecutive two‑digit
 *            * blocks, and interpreting each block as a decimal number with *
 *            leading zeros omitted
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t target   = n;     /* 1‑based index */
    idx_t num_done = 0;     /* number of pairs produced so far  */
    int   carry    = -1;    /* digit waiting to form a pair (-1 = none) */
    seq_t result   = 0;

    for (uint64_t num = 1;; ++num) {
        /* decompose num into its decimal digits (most significant first) */
        unsigned d[20];
        int      len = 0;
        uint64_t t   = num;
        while (t > 0) {
            d[len++] = t % 10;
            t /= 10;
        }
        if (len == 0) {          /* never happens for natural numbers */
            d[len++] = 0;
        }

        for (int i = len - 1; i >= 0; --i) {
            int digit = d[i];
            if (carry == -1) {
                carry = digit;
            } else {
                seq_t pair = (seq_t)carry * 10 + digit;
                if (carry == 0) pair = (seq_t)digit;   /* drop leading 0 */
                ++num_done;
                if (num_done == target) {
                    return pair;
                }
                carry = -1;
            }
        }
    }
}
