/*
 * A006589  a(n) = (n+3)*2^n - 1.
 * https://oeis.org/A006589
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)*2^n - 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"


seq_t a(idx_t n) {
    if (n == -1) {
        return 0;
    }
    seq_t n_plus_3 = (seq_t)(n + 3);
    seq_t pow2_n = A001832_pow2(n);
    return n_plus_3 * pow2_n - 1;
}
