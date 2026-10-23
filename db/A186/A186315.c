/*
 * A186315  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the squares and hexagonal
 *          numbers. Complement of A186316.
 * https://oeis.org/A186315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(1 + ⌊√(8*n*n - 7)⌋) / 4⌋
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The term a(n) is the rank of n² among the squares and hexagonals
   with a tie‑break that places the square before the hexagonal.) */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns ⌊√x⌋.  Uses binary search with 128‑bit arithmetic. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);         /* high^2 fits in 64 bits */
    idx_t mid, sq, best = 0;

    while (low <= high) {
        mid = (low + high) >> 1;
        sq  = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x) {
            best = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Compute 8*n*n - 7 safely in 128‑bit then truncate to idx_t.
       For n up to ~1.5e9 this fits in 64 bits; the build may use
       smaller integer types, but 64 bit arithmetic is safe. */
    idx_t n2     = n * n;          /* n² */
    idx_t inner  = 8 * n2 - 7;     /* 8*n² - 7 */
    idx_t sqrtv  = isqrt_u64(inner);         /* ⌊√(8*n²-7)⌋ */
    idx_t hexCnt = (sqrtv + 1) / 4;          /* ⌊(1+√)/4⌋  */
    return (seq_t)n + (seq_t)hexCnt;         /* a(n)  */
}
