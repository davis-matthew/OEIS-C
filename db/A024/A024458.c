/*
 * A024458  a(n) = s(1)*s(n) + s(2)*s(n-1) + ... + s(k)*s(n+1-k), where k =
 *          floor((n+1)/2), s = (Fibonacci numbers).
 * https://oeis.org/A024458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*m) = (m*Lucas(2*m+1) + Fibonacci(2*m)) / 5 * a(2*m+1) =
 *            ((-1)^m + (m+1)*Lucas(2*m+2) + Fibonacci(2*m+1)) / 5 * The
 *            implementation follows these formulas directly and uses 128‑bit
 *            * arithmetic (unsigned __int128) when seq_t is a 64‑bit unsigned
 *            type * to avoid overflow. For larger seq_t the bu...
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 176,
 *            big any n
 * Helpers:   fib_pair, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/fib_pair.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* n >= 1 is assumed (offset 1 in OEIS) */
    if ((n & 1U) == 0U) {                    /* even n = 2*m */
        idx_t m = n >> 1U;
        seq_t f_2m, f_2m1;                   /* Fib(2*m), Fib(2*m+1) */
        fib_pair(2U * m, &f_2m, &f_2m1);

        /* Lucas(2*m+1) = 2*Fib(2*m) + Fib(2*m+1) */
        seq_t lucas = mul_small(f_2m, 2U) + f_2m1;

        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            /* 64‑bit build: perform the multiplication in 128 bits */
            unsigned __int128 num = (unsigned __int128)(idx_t)m *
                                    (unsigned __int128)(uint64_t)lucas;
            num += (uint64_t)f_2m;                /* + Fibonacci(2*m) */
            return (seq_t)(num / 5U);
        } else {
            seq_t term = mul_small(lucas, (unsigned)m) + f_2m;
            return term / 5U;
        }
    } else {                                   /* odd n = 2*k+1 */
        idx_t k = (n - 1) >> 1U;               /* k = (n-1)/2 */
        seq_t f_2k1, f_2k2;                    /* Fib(2*k+1), Fib(2*k+2) */
        fib_pair(2U * k + 1U, &f_2k1, &f_2k2);

        /* Lucas(2*k+2) = 2*Fib(2*k+1) + Fib(2*k+2) */
        seq_t lucas = mul_small(f_2k1, 2U) + f_2k2;

        /* Numerator = (-1)^k + (k+1)*Lucas + Fib(2*k+1) */
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            unsigned __int128 num = (unsigned __int128)(k + 1U) *
                                    (unsigned __int128)(uint64_t)lucas;
            num += (uint64_t)f_2k1;
            if (k & 1U)                          /* (-1)^k = -1 */
                num -= 1U;
            else                                 /* (-1)^k = +1 */
                num += 1U;
            return (seq_t)(num / 5U);
        } else {
            seq_t term = mul_small(lucas, (unsigned)(k + 1U)) + f_2k1;
            if (k & 1U)
                term -= 1U;
            else
                term += 1U;
            return term / 5U;
        }
    }
}
