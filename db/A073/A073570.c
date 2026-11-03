/*
 * A073570  G.f.: Sum_{n >= 1} x^n/(1-x^n)^5.
 * https://oeis.org/A073570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} C(d+3,4) where C(k,4)=k*(k-1)*(k-2)*(k-3)/24
 *            The function is fully self–contained and works for any
 *            8,16,32,64 or 128‑bit unsigned seq_t, with idx_t an unsigned
 *            64‑bit index
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 555, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9856 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* for uint64_t, etc. */

/* Compute the binomial coefficient C(d+3,4) = (d+1)(d+2)(d+3)(d+4)/24 */
static inline seq_t pentatope(idx_t d)
{
    /* use 128‑bit intermediate to avoid overflow before truncation to seq_t */
    unsigned __int128 v = d;
    v *= (d + 1);
    v *= (d + 2);
    v *= (d + 3);
    v /= 24;
    return (seq_t)v;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; 0 is unused but harmless */
        return (seq_t)0;

    seq_t sum = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += pentatope(i);
            if (i != n / i)      /* add partner divisor if distinct */
                sum += pentatope(n / i);
        }
    }
    return sum;
}
