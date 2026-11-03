/*
 * A205507  a(n) = Fibonacci(n) * A004018(n) for n>=1 with a(0)=1, where
 *          A004018(n) is the number of ways of writing n as a sum of 2
 *          squares.
 * https://oeis.org/A205507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * A004018(n) where A004018(n) is the number
 *            of integer pairs (x, y) satisfying x^2 + y^2 = n (signs and
 *            order counted). This uses the prime‑factorisation formula r_2(n)
 *            = 4 * Π_{p≡1 (mod4)} (e_p + 1), provided no prime ≡3 (mod4)
 *            occurs to an odd exponent, otherwise r_2(n)=0
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 44, u64 n <= 89, u128 n <= 184,
 *            big any n
 * Validated: standard, pass, 66 terms through n = 984 (2026-11-01)
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

static inline seq_t sum_of_two_squares_cnt(idx_t m)
{
    if (m == 0)
        return 1;                 /* by definition of a(0) */
    seq_t prod = 1;
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if ((p % 4) == 3) {    /* primes 3 mod 4 */
                if (exp & 1u)   /* odd exponent ⇒ no representation */
                    return 0;
            } else if ((p % 4) == 1) { /* primes 1 mod 4 */
                prod *= (seq_t)(exp + 1);
            }
            /* p % 4 == 2 (p=2) is ignored */
        }
    }
    if (tmp > 1) {                    /* remaining prime factor > sqrt(m) */
        if ((tmp % 4) == 3)           /* odd exponent (1) of a 3 mod 4 prime */
            return 0;
        if ((tmp % 4) == 1)           /* prime 1 mod 4 with exponent 1 */
            prod *= (seq_t)2;
    }
    prod *= (seq_t)4;                 /* final factor of 4 for sign/order */
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 by definition */

    seq_t fib_n = fib(n);              /* Fibonacci(n) from utils */
    seq_t rep   = sum_of_two_squares_cnt(n);

    return fib_n * rep;
}
