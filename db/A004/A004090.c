/*
 * A004090  Sum of digits of Fibonacci numbers.
 * https://oeis.org/A004090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A000045(n)), the digit sum of Fibonacci(n), where
 *            Fibonacci(n) = Fibonacci(n-1) + Fibonacci(n-2), Fibonacci(0) =
 *            0, Fibonacci(1) = 1, held as exact decimal limbs
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Fibonacci(n) has about 0.209*n + 1 decimal digits; base 1e9 limbs */
    size_t limbs = (size_t)(n / 40) + 4;
    uint32_t *x = (uint32_t *)malloc(limbs * sizeof(uint32_t));
    uint32_t *y = (uint32_t *)malloc(limbs * sizeof(uint32_t));
    memset(x, 0, limbs * sizeof(uint32_t));
    memset(y, 0, limbs * sizeof(uint32_t));
    x[0] = 0; /* F(0) */
    y[0] = 1; /* F(1) */
    size_t used = 1;

    for (idx_t i = 2; i <= n; i++) {
        /* x <- x + y, then swap so y = F(i), x = F(i-1) */
        uint32_t carry = 0;
        size_t k;
        for (k = 0; k < used; k++) {
            uint32_t s = x[k] + y[k] + carry;
            if (s >= 1000000000u) { s -= 1000000000u; carry = 1; }
            else carry = 0;
            x[k] = s;
        }
        if (carry) { x[used] = carry; used++; }
        uint32_t *t = x; x = y; y = t;
    }

    seq_t digit_sum = 0;
    for (size_t k = 0; k < used; k++) {
        uint32_t v = y[k];
        while (v > 0) {
            digit_sum += (seq_t)(v % 10);
            v /= 10;
        }
    }
    free(x);
    free(y);
    return digit_sum;
}
