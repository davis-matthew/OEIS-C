/*
 * A095966  Numbers k such that k * (largest digit of k) is a square.
 * https://oeis.org/A095966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest k (k≥0) such that k * largest_digit(k) is a
 *            perfect square, counted from n = 1 with a(1) = 0
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_isqrt_seq, largest_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_digit.h"
#include "../utils/floor_isqrt_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t d = largest_digit(k_seq);           /* largest decimal digit of k */
        seq_t prod = k_seq * d;                   /* k × largest_digit(k) */
        seq_t r = floor_isqrt_seq(prod);          /* floor(sqrt(prod)) */
        if (r * r == prod) {                     /* prod is a perfect square? */
            ++count;
            if (count == n) {
                return k_seq;
            }
        }
    }
}
