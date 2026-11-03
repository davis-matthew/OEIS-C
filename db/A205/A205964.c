/*
 * A205964  a(n) = Fibonacci(n)*A000143(n) for n>=1 with a(0)=1, where
 *          A000143(n) is the number of ways of writing n as a sum of 8
 *          squares.
 * https://oeis.org/A205964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * A000143(n) where A000143(n) = number of
 *            ways of writing n as a sum of 8 squares. For n > 0 let σ₃(n) =
 *            Σ_{d|n} d³, and let odd(n) = n / 2^{v₂(n)} be the odd part of n.
 *            Then A000143(n) = 16 * σ₃(n) if n is odd, A000143(n) = 16 *
 *            σ₃(n) – 32 * σ₃(odd(n)) if n is even. For n = 0...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 22, u64 n <= 62, u128 n <= 149,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

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

static seq_t sigma3_u64(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d2 = (seq_t)(n / i);
            sum += d1 * d1 * d1;
            if (d1 != d2) {
                sum += d2 * d2 * d2;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* a(0) = 1 */

    seq_t sigma_n = sigma3_u64(n);
    seq_t r8;

    if (n & 1U) {                   /* n is odd */
        r8 = sigma_n * (seq_t)16;
    } else {                        /* n is even */
        idx_t odd = n;
        while ((odd & 1U) == 0) odd >>= 1;   /* remove all factors of 2 */
        seq_t sigma_odd = sigma3_u64(odd);
        r8 = sigma_n * (seq_t)16 - sigma_odd * (seq_t)32;
    }

    seq_t fib_n = fib(n);
    return fib_n * r8;
}
