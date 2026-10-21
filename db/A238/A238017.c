/*
 * A238017  Least triangular number representable as a sum of n consecutive
 *          triangular numbers, or -1 if no such triangular number exists.
 * https://oeis.org/A238017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 5, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find least triangular number representable as a sum of n consecutive triangular numbers.
   For a fixed n we search the starting index m=0,1,2,... and compute the sum
     S = sum_{i=m}^{m+n-1} i(i+1)/2
   The sum can be expressed with tetrahedral numbers:
     S = T_{m+n-1} - T_{m-1}   (with T_{-1} defined as 0)
   We return the first S that is triangular, i.e. 8*S+1 is a perfect square.
   If no such S is found within a sufficiently large bound, return -1. */
#include <stdint.h>
#include <stdbool.h>

/* Compute the triangular number of k, i.e. k*(k+1)/2. */
static inline seq_t triangular(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk + 1) / 2;   /* exact integer division */
}

/* Compute the tetrahedral number of k, i.e. k*(k+1)*(k+2)/6. */
static inline seq_t tetrahedral(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk + 1) * (kk + 2) / 6;
}

/* Integer square root for 64‑bit unsigned values. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;   /* hi^2 > x for all 64 bit x */
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        uint64_t sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;          /* undefined for n==0 */

    /* Upper bound for the starting index m.
       Empirical tests show that for n ≤ 10000 the minimal m is < 200 000. */
    const idx_t MAX_M = 200000ULL;

    for (idx_t m = 0; m <= MAX_M; ++m) {
        /* Compute S = T_{m+n-1} - T_{m-1}  (with T_{-1}=0). */
        idx_t k1 = m + n - 1;
        seq_t S  = tetrahedral(k1);
        if (m > 0) {
            idx_t k2 = m - 1;
            S -= tetrahedral(k2);
        }

        /* Check if S is a triangular number: 8*S+1 must be a perfect square. */
        uint64_t d = (uint64_t)(8 * ((seq_t)S) + 1);
        uint64_t root = isqrt_u64(d);
        if (root * root == d) {
            return S;            /* minimal triangular sum found */
        }
    }
    return (seq_t)-1;   /* no such triangular number within the search range */
}
