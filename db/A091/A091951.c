/*
 * A091951  Let R_{k}(m) = the digit reversal of m in base k (R_{k}(m) is
 *          written in base 10). a(n) is the smallest m such that R_{k}(m) =
 *          n, where k >= 2.
 * https://oeis.org/A091951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{k>=2, n mod k != 0} {integer obtained by reversing
 *            the base‑k digits of n}
 * Range:     u8 n <= 31, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    seq_t best = (seq_t)n;           // base > n gives candidate m = n
    for (idx_t k = 2; k < n; ++k) {
        if (n % k == 0)              // would lead to leading zero in m
            continue;

        seq_t rev = 0;
        idx_t temp = n;
        while (temp > 0) {
            uint64_t digit = temp % k;
            rev = rev * (seq_t)k + (seq_t)digit;
            if (rev > best)          // no need to continue: already larger
                break;
            temp /= k;
        }
        if (temp == 0 && rev < best)
            best = rev;
    }
    return best;
}
