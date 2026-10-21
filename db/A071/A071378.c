/*
 * A071378  Largest proper divisor of n^3.
 * https://oeis.org/A071378
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 / spf(n), where spf(n) is the smallest prime divisor
 *            of n. Equivalent to largest proper divisor of n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Smallest prime factor. Assumes n >= 2. */
static inline seq_t spf(idx_t n)
{
    if (n % 2 == 0) return (seq_t)2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return (seq_t)d;
    return (seq_t)n;          /* n is prime */
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;     /* undefined below offset; return 0 */

    seq_t sp = spf(n);          /* smallest prime factor of n */

    /* Compute n^3 in seq_t. All operations are safe for the tested range. */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;      /* n^2 */
    seq_t n_cube = n_sq * n_seq;       /* n^3 */

    return n_cube / sp;                 /* largest proper divisor */
}
