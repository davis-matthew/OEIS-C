/*
 * A182079  a(n) = floor(n*floor((n-1)/2)/3).
 * https://oeis.org/A182079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * floor((n-1)/2) / 3 )
 * Range:     u8 n <= 40, u16 n <= 628, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes OEIS A182079: a(n) = floor( n*floor((n-1)/2) / 3 )   */
seq_t a(idx_t n)
{
    /* Guard against n == 0,1,2 – the formula naturally yields 0 */
    if (n < 3)
        return (seq_t)0;

    /* Compute floor((n-1)/2) using integer division. */
    idx_t mid = (n - 1) / 2;

    /* Perform the multiplication and division in a wider type to avoid
       overflow for typical seq_t sizes (uint8_t, uint16_t, uint32_t,
       uint64_t, __int128).  Using unsigned long long provides enough
       range for n up to at least 10 000 (product < 5·10^7).  For C++ builds
       where seq_t is an arbitrary‑precision type, the conversion from
       unsigned long long is implicit. */
    unsigned long long n64 = n;
    unsigned long long mid64 = mid;
    unsigned long long prod = n64 * mid64;
    unsigned long long res64 = prod / 3;

    return (seq_t)res64;
}
