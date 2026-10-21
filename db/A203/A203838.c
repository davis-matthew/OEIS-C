/*
 * A203838  a(n) = sigma_3(n)*Fibonacci(n), where sigma_3(n) = A001158(n), the
 *          sum of cubes of divisors of n.
 * https://oeis.org/A203838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma_3(n) * Fib(n), sigma_3(n) = Σ_{d|n} d^3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t n); */

static inline seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

static seq_t sigma3(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t p_seq = (seq_t)p;
            seq_t num = pow_uint(p_seq, 3 * (exp + 1)) - 1;
            seq_t den = pow_uint(p_seq, 3) - 1;
            result *= num / den;
        }
    }
    if (temp > 1) {
        seq_t p_seq = (seq_t)temp;
        seq_t num = pow_uint(p_seq, 3 * 2) - 1;   /* exp = 1  → 3*(1+1) = 6 */
        seq_t den = pow_uint(p_seq, 3) - 1;
        result *= num / den;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                     /* out‑of‑range guard, offset starts at 1 */
    seq_t sig = sigma3(n);
    seq_t fibn = fib(n);
    return sig * fibn;
}
