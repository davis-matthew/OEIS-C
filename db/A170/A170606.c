/*
 * A170606  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 21 * 20^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: compute 20^e with binary exponentiation. */
static seq_t pow20(idx_t e) {
    seq_t result = 1;
    seq_t base   = 20;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t val = pow20(n - 1);
    val *= 21;
    return val;
}
