/*
 * A183907  Number of nondecreasing arrangements of n+2 numbers in 0..4 with
 *          each number being the sum mod 5 of two others.
 * https://oeis.org/A183907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 18 n^3 + 119 n^2 – 570 n + 552) / 24 for n > 1 *
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 180, u64 n <= 180,
 *            u128 n <= 180, big any n
 * Validated: standard, pass, 56 terms through n = 162 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute the polynomial in a type that does not overflow.
   For compilers supporting the 128‑bit unsigned integer, use it;
   otherwise fall back to 64‑bit arithmetic (which suffices for n up to ~10000). */
static inline seq_t polynomial_value(idx_t n)
{
    if (n == 1) return (seq_t)1;
    /* Use unsigned 128‑bit if available */
#ifdef __SIZEOF_INT128__
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;           /* n^2 */
    unsigned __int128 n3 = n2 * N;          /* n^3 */
    unsigned __int128 n4 = n3 * N;          /* n^4 */
    unsigned __int128 num = n4 + 18 * n3 + 119 * n2 - 570 * N + 552;
    unsigned __int128 res = num / 24;
    return (seq_t)res;
#else
    /* 64‑bit build: for n ≤ 10000, all intermediate results fit in uint64_t */
    uint64_t N  = (uint64_t)n;
    uint64_t n2 = N * N;         /* n^2 */
    uint64_t n3 = n2 * N;        /* n^3 */
    uint64_t n4 = n3 * N;        /* n^4 */
    uint64_t num = n4 + 18 * n3 + 119 * n2 - 570 * N + 552;
    uint64_t res = num / 24;
    return (seq_t)res;
#endif
}

seq_t a(idx_t n)
{
    return polynomial_value(n);
}
