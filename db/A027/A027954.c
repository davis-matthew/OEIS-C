/*
 * A027954  a(n) = T(2n+1, n+2), T given by A027948.
 * https://oeis.org/A027954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥2, a(n)=Fibonacci(2n+6)−(24+23n+12n²+4n³)/3
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle offset and trivial cases */
    if (n == 0)
        return (seq_t)0;   /* value below the offset – arbitrary */
    if (n == 1)
        return (seq_t)1;

    /* Compute Fibonacci(2n+6) */
    idx_t idx = 2 * n + 6;
    seq_t fib_val = fib(idx);

    /* Compute the polynomial part: (24 + 23n + 12n² + 4n³)/3 */
    seq_t a   = (seq_t)n;
    seq_t a2  = a * a;         /* n² */
    seq_t a3  = a2 * a;        /* n³ */
    seq_t term = 4 * a3 + 12 * a2 + 23 * a + 24;
    seq_t term_div3 = term / 3;

    /* Result */
    return fib_val - term_div3;
}
