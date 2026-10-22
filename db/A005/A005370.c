/*
 * A005370  a(n) = Fibonacci(Fibonacci(n+1) + 1).
 * https://oeis.org/A005370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(Fibonacci(n+1) + 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   mat_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mat_mult.h"

static seq_t fibonacci(seq_t k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    if (k == 2) return 1;
    
    seq_t base[4] = {1, 1, 1, 0};
    seq_t result[4] = {1, 0, 0, 1};
    
    seq_t exp = k;
    while (exp > 0) {
        if (exp & 1) {
            seq_t tmp[4];
            mat_mult(result, base, tmp);
            result[0] = tmp[0];
            result[1] = tmp[1];
            result[2] = tmp[2];
            result[3] = tmp[3];
        }
        exp >>= 1;
        if (exp > 0) {
            seq_t tmp[4];
            mat_mult(base, base, tmp);
            base[0] = tmp[0];
            base[1] = tmp[1];
            base[2] = tmp[2];
            base[3] = tmp[3];
        }
    }
    
    return result[1];
}

seq_t a(idx_t n) {
    seq_t fib_n_plus_1 = fibonacci((seq_t)(n + 1));
    return fibonacci(fib_n_plus_1 + 1);
}
