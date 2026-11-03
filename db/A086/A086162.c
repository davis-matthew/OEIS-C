/*
 * A086162  Number of monomial ideals in two variables x, y that are Artinian,
 *          integrally closed, of colength n and contain x^3.
 * https://oeis.org/A086162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 1528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9855 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086162: Number of monomial ideals in two variables x,y that are
   Artinian, integrally closed, of colength n and contain x^3.
   The generating function is
        (1+t^2+t^5-2*t^6-t^8+t^9)/((1-t)*(1-t^3)*(1-t^6)).
   From this we obtain the closed form
        a(n)=f(n)+f(n-2)+f(n-5)-2*f(n-6)-f(n-8)+f(n-9),
   where f(k) is the number of non‑negative integer solutions of
        a + 3b + 6c = k.
   f(k)=0 for k<0.  This helper is computed multiplicatively, avoiding
   any factorial overflows.  */

#include <stdint.h>

static inline seq_t count_f(int64_t k)
{
    if (k < 0)
        return 0;
    seq_t total = 0;
    for (int64_t c = 0; 6 * c <= k; ++c)
    {
        int64_t rem = k - 6 * c;
        total += (seq_t)(rem / 3 + 1);
    }
    return total;
}

seq_t a(idx_t n)
{
    /* All subtractions are performed on signed integers to avoid unsigned wrap‑around. */
    int64_t N = (int64_t)n;
    seq_t val = count_f(N)
              + count_f(N - 2)
              + count_f(N - 5)
              - 2 * count_f(N - 6)
              - count_f(N - 8)
              + count_f(N - 9);
    return val;
}
