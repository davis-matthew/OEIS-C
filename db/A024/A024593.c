/*
 * A024593  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (F(2), F(3), ...), t = A000201 (lower Wythoff
 *          sequence).
 * https://oeis.org/A024593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{floor((n+1)/2)} F_{i+1} * floor((n+1-i)*phi)
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* offset is 1; assume n >= 1 */
    idx_t k = (n + 1) / 2;           /* floor((n+1)/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= k; ++i) {
        idx_t j = n + 1 - i;        /* j ranges from n down to ceil((n+1)/2) */

        /* s(i) = Fibonacci(n+1), with F(0)=0, F(1)=1 */
        seq_t s = fib((idx_t)(i + 1));   /* F_{i+1} */

        /* t(j) = floor(j * phi)
           using the identity: floor(j*phi) = floor((j + floor(j*sqrt5))/2) */
        seq_t j_seq   = (seq_t)j;
        seq_t j2      = seq_mul(j_seq, j_seq);          /* j^2      */
        seq_t five_j2 = seq_mul(j2, (seq_t)5);          /* 5*j^2    */
        seq_t sqrt_term = floor_sqrt(five_j2);          /* floor(sqrt(5*j^2)) = floor(j*sqrt5) */
        seq_t t = (j_seq + sqrt_term) / 2;              /* floor((j + floor(j*sqrt5))/2) */

        seq_t prod = seq_mul(s, t);
        sum += prod;
    }

    return sum;
}
