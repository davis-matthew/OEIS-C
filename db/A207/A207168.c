/*
 * A207168  Number of n X 7 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*n^4 + 42*n^3 + 7*n^2 - 84*n + 28
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 104, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value below offset is irrelevant */
    long long N   = (long long)n;   /* n fits in a signed 64‑bit type here */
    long long n2  = N * N;
    long long n3  = n2 * N;
    long long n4  = n3 * N;
    long long val = 35LL * n4 + 42LL * n3 + 7LL * n2 - 84LL * N + 28LL;
    return (seq_t)val;
}
