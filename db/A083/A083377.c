/*
 * A083377  a(n) = the largest integer whose square has n digits and first
 *          digit 1.
 * https://oeis.org/A083377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   a(n) = floor(sqrt(10^n/5)) = floor(sqrt(2*10^(n-1)))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 28,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

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
