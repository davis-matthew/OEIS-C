/*
 * A113196  a(n) = F(n)/Product_{p=primes} F(p^(m_{n,p})), where p^(m_{n,p})
 *          is highest power of p dividing n, m= nonnegative integer and F(k)
 *          is the k-th Fibonacci number.
 * https://oeis.org/A113196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n) / ∏_{p^e‖n} F(p^e) (F(k) = k-th Fibonacci number)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 47, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    /* For n < 1 the OEIS definition has no meaning; return 0 as a guard. */
    if (n == 0) return 0;

    /* Compute F(n).  `fib_fast` returns F(n) in `Fn` and F(n+1) in `Fn_next`. */
    seq_t Fn, Fn_next;
    fib_fast(n, &Fn, &Fn_next);

    /* `product` will hold the denominator ∏ F(p^e).  Start at 1. */
    seq_t product = 1;

    /* Work on a copy of n to factor it. */
    idx_t m = n;

    /* Handle the factor 2 separately (fast shift). */
    if ((m & 1) == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        idx_t p_pow = 1;
        for (idx_t i = 0; i < e; ++i) p_pow *= 2;   /* 2^e */
        seq_t Fp;
        fib_fast(p_pow, &Fp, &Fn_next);
        product = product * Fp;
    }

    /* Trial division for odd primes up to √m. */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            idx_t p_pow = 1;
            for (idx_t i = 0; i < e; ++i) p_pow *= p;   /* p^e */
            seq_t Fp;
            fib_fast(p_pow, &Fp, &Fn_next);
            product = product * Fp;
        }
    }

    /* Remaining factor (prime > sqrt(m) or m==1). */
    if (m > 1) {
        seq_t Fp;
        fib_fast(m, &Fp, &Fn_next);
        product = product * Fp;
    }

    /* Return the quotient.  `seq_t` supports integer division. */
    return Fn / product;
}
