/*
 * A083500  Smallest k such that n*(n+k) + 1 is a cube.
 * https://oeis.org/A083500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A083500 – Smallest k such that n*(n+k)+1 is a cube. */
/* Algorithm:  For each n, let m be the least integer with m³>n² that satisfies
 * (m³−1) divisible by n. Then k = (m³−1)/n − n. The routine searches for such
 * an m by starting at ceil(cuberoot(n²)) and testing successive integers.
 * All arithmetic is performed with the sequence type seq_t, so the code works
 * for built‑in integer types, unsigned __int128, and arbitrary‑precision
 * integer classes.  No casts to built‑in integral types are used.  */

#include <stdint.h>
#include <stdbool.h>

/*--------------------------------------------------------------------*/
/* Integer cube root (floor).  Works for arbitrary seq_t.             */
/*--------------------------------------------------------------------*/
static inline seq_t cube_root_floor(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)0;
    seq_t low  = (seq_t)0;
    seq_t high = (seq_t)1;                 /* high is 2ʰ where 2ʰ³ > x */

    /* Exponentially increase high until high³ > x */
    while (high*high*high <= x) {
        high <<= 1;
    }

    /* Binary search on [low, high) */
    while (high > low + (seq_t)1) {
        seq_t mid  = (low + high) >> 1;
        seq_t mid3 = mid*mid*mid;
        if (mid3 <= x)
            low = mid;
        else
            high = mid;
    }

    return low;
}

/*--------------------------------------------------------------------*/
/* Main sequence function.                                            */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n >= 1 (offset 1).  Compute n². */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Find smallest m with m³ > n². */
    seq_t m = cube_root_floor(sq) + (seq_t)1;

    /* Search for m such that (m³−1) is divisible by n. */
    for (;;) {
        seq_t m3   = m*m*m;
        seq_t t    = m3 - (seq_t)1;
        if (t % (seq_t)n == (seq_t)0) {
            seq_t k = (t / (seq_t)n) - (seq_t)n;
            return k;
        }
        ++m;
    }
}
