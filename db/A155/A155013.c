/*
 * A155013  Integer part of square root of n^5 = A000584(n).
 * https://oeis.org/A155013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2 * sqrt(n))
 * Range:     u8 n <= 9, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes floor(sqrt(n^5)) for n >= 1.
   Uses unsigned __int128 for intermediate arithmetic, which is
   available in all common GCC/Clang compilers. The result fits in
   64 bits for n up to 10000, which is sufficient for the benchmark. */
seq_t a(idx_t n)
{
    /* n must be >= 1; behaviour for n == 0 is undefined */
    unsigned __int128 n128 = (unsigned __int128)n;

    /* Compute n^5 as a 128‑bit integer */
    unsigned __int128 n5 = n128 * n128;          /* n^2   */
    n5 = n5 * n128;                              /* n^3   */
    n5 = n5 * n128 * n128;                       /* n^5   */

    /* Binary search for floor(sqrt(n5)).
       The result always fits in uint64_t for the tested range. */
    uint64_t low  = 0;
    uint64_t high = ((uint64_t)1) << 34;          /* > sqrt(10^20) */

    /* Ensure high is large enough (usually already is). */
    while ((unsigned __int128)high * high <= n5) {
        if (high > ((uint64_t)1ULL << 63)) {
            high = (uint64_t)-1;
            break;
        }
        high <<= 1;
    }

    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= n5)
            low = mid;
        else
            high = mid - 1;
    }

    return (seq_t)low;
}
