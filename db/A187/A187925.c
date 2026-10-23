/*
 * A187925  Coefficient of x^n in (1+x+x^2+x^3+x^4)^n.
 * https://oeis.org/A187925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..⌊n/5⌋} (-1)^i * binom(n,i) * * binom(2n-5i-1,
 *            n-5i)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 64,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the coefficient of x^n in (1+x+x^2+x^3+x^4)^n */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    /* Use a big‑int path when seq_t can hold more than 8 bytes,
       otherwise fall back to 128‑bit intermediate arithmetic. */
    if (sizeof(seq_t) > 8) {
        /* arbitrary‑precision implementation */
        seq_t sum = (seq_t)0;
        idx_t max_i = n / 5u;
        for (idx_t i = 0u; i <= max_i; ++i) {
            seq_t term = binom(n, i);

            idx_t k = n - 5u * i;
            idx_t N = 2u * n - 5u * i - 1u;

            /* compute binom(N, k) with arbitrary precision */
            idx_t kk = k;
            if (kk > N - kk) kk = N - kk;
            seq_t bin2 = (seq_t)1;
            for (idx_t j = 1u; j <= kk; ++j) {
                bin2 = bin2 * (N - kk + j) / j;
            }

            term = term * bin2;
            if (i & 1u) sum -= term;
            else         sum += term;
        }
        return sum;
    } else {
        /* 64‑bit build: use 128‑bit intermediate to avoid overflow
           while n stays small enough for the result to fit in 64 bits. */
        __int128 sum = 0;
        idx_t max_i = n / 5u;
        for (idx_t i = 0u; i <= max_i; ++i) {
            __int128 term = (__int128)binom(n, i);

            idx_t k = n - 5u * i;
            idx_t N = 2u * n - 5u * i - 1u;

            idx_t kk = k;
            if (kk > N - kk) kk = N - kk;
            __int128 bin2 = 1;
            for (idx_t j = 1u; j <= kk; ++j) {
                bin2 = bin2 * (__int128)(N - kk + j) / (__int128)j;
            }

            term = term * bin2;
            if (i & 1u) sum -= term;
            else         sum += term;
        }
        return (seq_t)sum;
    }
}
