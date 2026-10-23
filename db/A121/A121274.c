/*
 * A121274  a(n) = n-th integer from among those positive integers which are
 *          coprime to (n-th integer from among those positive integers which
 *          are coprime to n).
 * https://oeis.org/A121274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer coprime to (nth integer coprime to n)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t nth_coprime(seq_t target, idx_t rank) {
    idx_t count = 0;
    idx_t candidate = 1;
    while (1) {
        /* gcd of candidate and target */
        idx_t g = (idx_t)gcd((seq_t)candidate, target);
        if (g == 1) {
            count++;
            if (count == rank) {
                return (seq_t)candidate;
            }
        }
        candidate++;
    }
}

seq_t a(idx_t n) {
    seq_t m = nth_coprime((seq_t)n, n);
    return nth_coprime(m, n);
}
