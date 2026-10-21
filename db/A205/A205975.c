/*
 * A205975  a(n) = Fibonacci(n)*A002652(n) for n>=1, with a(0)=1, where
 *          A002652 lists the coefficients in theta series of Kleinian lattice
 *          Z[(-1+sqrt(-7))/2].
 * https://oeis.org/A205975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * A002652(n)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   fib, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t x) */
#include "../utils/fib.h"          /* fib(idx_t n) */

/* Count representations of n by the norm a^2 + a*b + 2*b^2
   (A002652(n)).  Assumes n > 0.  */
static seq_t count_representations(idx_t n)
{
    seq_t total   = 0;
    seq_t four_n  = (seq_t)4 * (seq_t)n;   /* 4*n */

    /* ---- b = 0 ---- */
    seq_t Δ          = four_n;
    seq_t sqrtΔ      = floor_sqrt(Δ);
    if (sqrtΔ * sqrtΔ == Δ)               /* Δ perfect square */
        total += 2;                       /* a = ± sqrtΔ/2 */

    /* ---- b = ±y,   y > 0 ---- */
    for (idx_t y = 1;; ++y)
    {
        seq_t ysq   = (seq_t)y * (seq_t)y;
        seq_t need  = (seq_t)7 * ysq;
        if (need > four_n)
            break;

        seq_t Δy   = four_n - need;
        seq_t sqrtΔy = floor_sqrt(Δy);
        if (sqrtΔy * sqrtΔy != Δy)
            continue;                      /* not a perfect square */

        /* parity check: (y + sqrtΔy) must be even */
        if (((seq_t)y + sqrtΔy) & 1)
            continue;

        if (Δy == 0)
            total += 2;                   /* one solution for each sign of b */
        else
            total += 4;                   /* two a for each sign of b */
    }

    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                   /* a(0) = 1 */

    seq_t fib_n = fib(n);                  /* Fibonacci(n) */
    seq_t reps  = count_representations(n);/* A002652(n)  */
    return fib_n * reps;
}
