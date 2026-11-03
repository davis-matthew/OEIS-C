/*
 * A185593  a(n) = floor(n^(3/2))*floor(3+n^(3/2))/2.
 * https://oeis.org/A185593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2))*floor(3+n^(3/2))/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 2046, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 73 terms through n = 4994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n^3)) for a given idx_t n.
 * Uses binary search on 128‑bit product to avoid overflow
 * for the typical values of n used in the sequence. */
static seq_t floor_n3_sqrt(idx_t n)
{
    if (n == 0)
        return 0;

    /* product = n^3  stored in 128 bit to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)n * n * n;

    /* The square root of n^3 is at most n^2.
       For the ranges of interest (n ≤ 10⁴ etc.) n² fits in 64 bits,
       so the binary search bounds fit in unsigned long long. */
    unsigned long long low  = 0;
    unsigned long long high = (unsigned long long)n * n + 1;  /* exclusive upper bound */

    while (low + 1 < high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= prod)
            low = mid;
        else
            high = mid;
    }

    return (seq_t)low;
}

seq_t a(idx_t n)
{
    /* sequence offset is 1, so n≥1 in practice */
    seq_t t = floor_n3_sqrt(n);          /* floor(n^(3/2)) */
    seq_t res = t * (t + 3) / 2;         /* floor(n^(3/2)) * floor(3+n^(3/2)) / 2 */
    return res;
}
