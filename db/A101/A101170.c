/*
 * A101170  a(n) divides the number formed by concatenating the sum of the
 *          digits of a(n) with a(n).
 * https://oeis.org/A101170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer a such that a divides concat(
 *            digit_sum(a), a ) = digit_sum(a)*10^digits(a) + a The sequence
 *            is generated in increasing order
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   digit_sum, digits_seq, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/digits_seq.h"
#include "../utils/mulmod.h"

seq_t a(idx_t n) {
    static idx_t found = 0;       /* how many terms have been found so far */
    static seq_t cur   = 0;       /* current candidate a */

    /* generate terms until we reach the n‑th one */
    while (found < n) {
        ++cur;                    /* start from 1, then 2, 3, ... */

        seq_t s = digit_sum(cur);           /* sum of digits of cur */
        idx_t d = (idx_t)digits_seq(cur);   /* number of decimal digits */

        /* compute 10^d mod cur - do not overflow */
        seq_t pow10_mod = 1;
        for (idx_t i = 0; i < d; ++i)
            pow10_mod = mulmod(pow10_mod, 10, cur);

        /* a divides s*10^d iff (s*pow10_mod) mod cur == 0 */
        seq_t prod_mod = mulmod(s % cur, pow10_mod, cur);
        if (prod_mod == 0) {
            ++found;
            if (found == n)
                return cur;
        }
    }
    /* unreachable, but keep compiler happy */
    return cur;
}
