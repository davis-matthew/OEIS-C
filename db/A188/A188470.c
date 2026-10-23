/*
 * A188470  a(n) = [5r]-[nr]-[5r-nr], where r=(1+sqrt(5))/2 and []=floor.
 * https://oeis.org/A188470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [5r] - [nr] - [5r-nr], r = (1+sqrt(5))/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_n_phi, floor_phi_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_n_phi.h"
#include "../utils/floor_phi_signed.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* behavior below offset is irrelevant */

    const seq_t FLOOR_5R = 8;          /* floor(5r) = 8 */
    int64_t fnr    = (int64_t)floor_n_phi(n);   /* floor(n r)   */
    int64_t f5nmnr = floor_phi_signed((int64_t)5 - (int64_t)n); /* floor((5-n)r) */

    int64_t result = (int64_t)FLOOR_5R - fnr - f5nmnr;
    return (seq_t)result;                 /* 0 or 1 */
}
