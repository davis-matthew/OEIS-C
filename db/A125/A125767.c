/*
 * A125767  Least prime of the form n*T_i +/-1, where T is a triangular
 *          number.
 * https://oeis.org/A125767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime of n*T_i ± 1, where T_i = i*(i+1)/2
 * Range:     u8 n <= 30, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; value for n=0 not defined */
    for (idx_t i = 1; ; ++i) {
        seq_t t = (seq_t)i * ((seq_t)i + 1) / 2;  /* triangular number T_i */
        seq_t v1 = (seq_t)n * t - (seq_t)1;       /* n*T_i - 1 */
        seq_t v2 = (seq_t)n * t + (seq_t)1;       /* n*T_i + 1 */

        if (v1 >= (seq_t)2 && is_prime_big(v1)) return v1;
        if (v2 >= (seq_t)2 && is_prime_big(v2)) return v2;
    }
}
