/*
 * A101099  Third partial sums of fifth powers (A000584).
 * https://oeis.org/A101099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n*(n+2)-1)*(n*(n+4)+2)/336
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle offset: a(1) is the first term */
    if (n == 0) return 0;

    /* Use 128‑bit arithmetic to avoid overflow when intermediate products
       exceed 64 bits, but the final result fits into seq_t when it is
       supposed to.  Use unsigned __int128 which is supported by GCC/Clang
       in C11 and C++17. */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 f1 = N;                   /* n            */
    unsigned __int128 f2 = N + 1;               /* n+1          */
    unsigned __int128 f3 = N + 2;               /* n+2          */
    unsigned __int128 f4 = N + 3;               /* n+3          */
    unsigned __int128 f5 = N * (N + 2) - 1;      /* n*(n+2)-1    */
    unsigned __int128 f6 = N * (N + 4) + 2;      /* n*(n+4)+2    */

    /* Compute the product carefully in 128‑bit */
    unsigned __int128 prod = f1 * f2;
    prod *= f3;
    prod *= f4;
    prod *= f5;
    prod *= f6;

    /* Exact division by 336 in 128‑bit */
    prod /= 336;

    /* Convert back to seq_t; this cast truncates only if the result
       does not fit in seq_t, which, according to the build settings,
       happens only when seq_t is a fixed‑size type and the value
       exceeds its limit – in that case the build does not test that
       case for that seq_t. */
    return (seq_t)prod;
}
