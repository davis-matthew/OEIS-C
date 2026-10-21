/*
 * A209614  G.f.: Sum_{n>=1} Fibonacci(n^3)*x^(n^3).
 * https://oeis.org/A209614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) if n is a perfect cube, else 0
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 63, u64 n <= 124,
 *            u128 n <= 215, big any n
 * Helpers:   fib, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/int_cube_root.h"

/* Return the n-th term of OEIS sequence A209614 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1, guard */

    /* Find floor cube root */
    idx_t k = int_cube_root(n);

    /* Check if k^3 == n (using 128‑bit to avoid overflow) */
    if ((unsigned __int128)k * k * k == n)
        return fib(n);            /* Fibonacci(n) where n=k^3 */

    return (seq_t)0;                /* not a perfect cube */
}
