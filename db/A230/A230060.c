/*
 * A230060  Numbers k such that the distance from k^2 to the smallest
 *          triangular number >= k^2 is itself triangular.
 * https://oeis.org/A230060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that the distance from k² to the smallest
 *            triangular number ≥ k² is itself triangular
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;

    while (1) {
        /* k² */
        seq_t k2 = (seq_t)k * (seq_t)k;

        /* v = floor(√(1 + 8*k²)) */
        seq_t v = floor_sqrt((seq_t)1 + (seq_t)8 * k2);

        /* t_floor = floor((v - 1)/2) : largest t with T_t ≤ k² */
        seq_t t_floor = (v - (seq_t)1) / (seq_t)2;

        /* triangular number T_floor = t_floor*(t_floor+1)/2 */
        seq_t T_floor = t_floor * (t_floor + (seq_t)1) / (seq_t)2;

        /* Determine smallest triangular number ≥ k² */
        seq_t t = (T_floor == k2) ? t_floor : t_floor + (seq_t)1;
        seq_t T_t   = t * (t + (seq_t)1) / (seq_t)2;

        /* distance d = T_t - k² */
        seq_t d = T_t - k2;

        /* check if d is triangular: 8*d + 1 must be a perfect square */
        seq_t test = (seq_t)1 + (seq_t)8 * d;
        seq_t s = floor_sqrt(test);
        if (s * s == test) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
