/*
 * A037374  Positive numbers k such that every base-2 digit of k is a base-5
 *          digit of k.
 * https://oeis.org/A037374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer k such that every base‑2 digit of k
 *            is a base‑5 digit of k. The rule is equivalent to: the base‑5
 *            representation of k contains the digit 1, and if k’s binary
 *            representation contains a 0 (i.e. k is not of the form 2^m–1)
 *            then the base‑5 representation must also contain...
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand = 1;

    while (1) {
        /* Determine if binary representation of cand contains a 0.
           This is true unless cand is 2^m–1, i.e. unless cand+1 is a power of two. */
        bool has_zero_in_bin = ((cand & (cand + 1)) != 0);

        /* Scan base‑5 digits of cand.  We need to find at least one digit
           '1' and, if has_zero_in_bin is true, also a digit '0'. */
        seq_t tmp = cand;
        bool has1 = false;
        bool has0 = false;
        while (tmp > 0) {
            seq_t d = tmp % 5;
            if (d == 1) has1 = true;
            if (d == 0) has0 = true;
            tmp /= 5;
        }

        if (has1 && (!has_zero_in_bin || has0)) {
            count++;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
