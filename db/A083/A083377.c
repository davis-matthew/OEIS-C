/*
 * A083377  a(n) = the largest integer whose square has n digits and first
 *          digit 1.
 * https://oeis.org/A083377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   a(n) = floor(sqrt(10^n/5)) = floor(sqrt(2*10^(n-1)))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 932 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* index offset not used */
    /* Branch for small integer types (uint8_t … uint64_t) */
    if (sizeof(seq_t) <= 8) {
        /* Compute rhs = 2 * 10^(n-1) as a 128‑bit integer */
        idx_t exp = n - 1;
        unsigned __int128 rhs = 2;
        for (idx_t i = 0; i < exp; ++i) rhs *= 10;

        /* Binary search for floor(sqrt(rhs)) */
        unsigned __int128 low = 0, high = (unsigned __int128)~(uint64_t)0;
        while (low <= high) {
            unsigned __int128 mid = (low + high) >> 1;
            unsigned __int128 sq  = mid * mid;
            if (sq <= rhs) {
                low = mid + 1;
            } else {
                if (mid == 0) break;
                high = mid - 1;
            }
        }
        return (seq_t)high;
    }

    /* Branch for arbitrary‑precision seq_t */
    idx_t k;
    seq_t num;
    if (n & 1) {                      /* n is odd   */
        k = (n - 1) / 2;
        num = pow10(2 * k);           /* 10^(2k)    */
        num = num * ((seq_t)2);       /* *2         */
    } else {                          /* n is even  */
        k = (n - 2) / 2;
        num = pow10(2 * k);           /* 10^(2k)    */
        num = num * ((seq_t)20);      /* *20        */
    }
    return floor_sqrt(num);
}
