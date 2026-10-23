/*
 * A039158  Numbers whose base-11 representation has the same number of 0's
 *          and 3's.
 * https://oeis.org/A039158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑11 representation
 *            contains * as many 0’s as 3’s. This implementation performs a
 *            linear scan, testing * each integer in turn until the desired
 *            term is found
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t matched = 0;          /* number of terms found so far   */
    idx_t candidate = 1;        /* current integer to test         */
    while (1) {
        seq_t val = candidate;
        idx_t zero = 0, three = 0;
        seq_t tmp = val;
        /* Count 0’s and 3’s in base‑11 */
        while (tmp > 0) {
            uint64_t d = (uint64_t)(tmp % (seq_t)11);
            if (d == 0) zero++;
            else if (d == 3) three++;
            tmp = tmp / (seq_t)11;
        }
        if (zero == three) {
            matched++;
            if (matched == n)
                return val;
        }
        candidate++;            /* move to next integer            */
    }
}
