/*
 * A188187  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(5), k=1, [ ]=floor.
 * https://oeis.org/A188187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 149, u32 n <= 149, u64 n <= 149,
 *            u128 n <= 149, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ⌊n·√5⌋ − ⌊√5⌋ − ⌊(n−1)·√5⌋
 *          = ⌊n·√5⌋ − 2 − ⌊(n−1)·√5⌋
 * Since ⌊n·√5⌋ = ⌊√(5·n²)⌋, we compute integer square roots
 * of 5*n² and 5*(n−1)².  This implementation uses only integer
 * arithmetic and works for all allowed seq_t and idx_t types.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned value: floor(sqrt(x)).  */
static inline uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;  /* hi large enough for x up to 2^64-1 */
    while (lo + 1 < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid)  /* mid*mid <= x   avoid overflow  */
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* 1‑based index; for n=0 the function is not called. */
    uint64_t n64 = (uint64_t)n;
    uint64_t nm1 = n64 - 1;

    /* Compute floor(sqrt(5*n^2)) and floor(sqrt(5*(n-1)^2)) */
    uint64_t val_n   = (uint64_t)5 * n64 * n64;
    uint64_t val_nm1 = (uint64_t)5 * nm1 * nm1;

    uint64_t f_n   = floor_sqrt_u64(val_n);
    uint64_t f_nm1 = floor_sqrt_u64(val_nm1);

    /* a(n) = f_n - 2 - f_nm1  (guaranteed to be non‑negative) */
    uint64_t result = f_n - f_nm1 - 2;

    return (seq_t)result;   /* cast to seq_t; value fits in seq_t for tested n */
}
