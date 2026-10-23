/*
 * A186073  Irregular triangle, read by rows, of primes with suffix n and
 *          digits "7" prepended, otherwise 0.
 * https://oeis.org/A186073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is defined by iterating over rows of an irregular triangle.
 *            For a row with suffix r (r = 1,2,3,…) the terms are the primes
 *            of the form 7…7r – that is, one or more leading 7‑digits
 *            followed by r, stopping as soon as the first composite is
 *            encountered. The flattened sequence is obtained by con...
 * Range:     u8 n <= 1, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t idx = 1;          /* current linear index */
    for (idx_t r = 1; ; ++r) {
        seq_t r_seq = (seq_t)r;
        int d = digit_len(r_seq);
        seq_t pow10_d = pow10((idx_t)d);
        seq_t leading = 0;          /* leading 7‑digits as a number */
        bool found = false;        /* whether this row produced any prime */

        for (;;) {
            /* prepend one more 7 */
            leading = leading * 10 + 7;
            seq_t val = leading * pow10_d + r_seq;

            if (is_prime_big(val)) {
                if (idx == n) return val;
                ++idx;
                found = true;
            } else {
                /* first composite ends the row */
                break;
            }
        }

        if (!found) {              /* row had no primes -> output 0 */
            if (idx == n) return 0;
            ++idx;
        }

        if (idx > n) break;        /* safety: we have already returned */
    }

    /* should never be reached for valid n */
    return 0;
}
