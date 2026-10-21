/*
 * A078993  Starting at the chess position shown, a(n) is the number of ways
 *          Black can make n consecutive moves, followed by a checkmate in one
 *          move by White.
 * https://oeis.org/A078993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 0 for n = 0 or n = 1, 3 - 2^(n/2+2) + F(n+3) for even
 *            n, n > 0, 2 × (F(n-2) – 1) for odd n ≥ 3 } where F(k) is the
 *            k‑th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   fib_seq, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The required utility functions are provided in the utils library. */
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/fib_seq.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* Even indices: a(2*m)  (m > 0) */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;                     /* m = n/2,  m > 0 */
        seq_t pow2_val = pow2((m) + 2);      /* 2^(m+2) */

        idx_t fibIndex = n + 3;              /* F(n+3) */
        seq_t fib_val, unused;
        fib_seq(fibIndex, &fib_val, &unused);

        seq_t sum = fib_val + (seq_t)3;       /* fib + 3 */
        if (pow2_val > sum)
            return (seq_t)0;                 /* should not happen, safety */
        return sum - pow2_val;               /* a(n) = fib + 3 - pow2 */
    }

    /* Odd indices: a(2*m+1) where m ≥ 1 (n ≥ 3) */
    {
        idx_t fibIndex = n - 2;              /* F(n-2) */
        seq_t fib_val, unused;
        fib_seq(fibIndex, &fib_val, &unused);

        /* subtract 1 (fib_val ≥ 1 for n ≥ 3) */
        seq_t res = fib_val - (seq_t)1;
        return res + res;                    /* 2 × (fib - 1) */
    }
}
