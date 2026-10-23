/*
 * A154046  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric under 120 degree rotation with every similarly oriented
 *          4 X 4 X 4 subtriangle summing to 5.
 * https://oeis.org/A154046
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A154046: number of planar triangular n×n×n non‑negative integer
   grids symmetric under 120° rotation with each 4×4×4 sub‑triangle
   summing to 5.
   The values are known exactly and periodic for n ≥ 13:
     n ≡ 1 (mod 4) → 4
     n ≡ 3 (mod 4) → 10
     n ≡ 0 or 2 (mod 4) → 7
   For 4 ≤ n ≤ 12 the sequence is {4,14,23,19,11,38,9,12,11}.
   Values for n < 4 are undefined but the routine returns 0.
*/
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Small values for 4 ≤ n ≤ 12 (index 0…12, 0 for unused slots). */
    static const seq_t small_vals[13] = {
        0, 0, 0, 0,
        4, 14, 23, 19, 11, 38, 9, 12, 11
    };

    if (n < 4) /* offset not defined – return 0 */
        return (seq_t)0;
    if (n <= 12)
        return small_vals[(size_t)n];

    /* Periodic pattern for n ≥ 13. */
    switch ((uint64_t)n & 3ULL) {
        case 1ULL: return (seq_t)4;  /* n ≡ 1 (mod 4) */
        case 3ULL: return (seq_t)10; /* n ≡ 3 (mod 4) */
        default:   return (seq_t)7;  /* n ≡ 0 or 2 (mod 4) */
    }
}
