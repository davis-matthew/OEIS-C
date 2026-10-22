/*
 * A001088  Product of totient function: a(n) = Product_{k=1..n} phi(k) (cf.
 *          A000010).
 * https://oeis.org/A001088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product_{k=1..n} phi(k)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    static idx_t memo_n = 0;
    static seq_t memo_result = 1;
    
    if (n < memo_n) {
        seq_t result = 1;
        for (idx_t k = 1; k <= n; k++) {
            result *= (seq_t)phi(k);
        }
        return result;
    } else if (n == memo_n) {
        return memo_result;
    } else {
        seq_t result = memo_result;
        for (idx_t k = memo_n + 1; k <= n; k++) {
            result *= (seq_t)phi(k);
        }
        memo_n = n;
        memo_result = result;
        return result;
    }
}
