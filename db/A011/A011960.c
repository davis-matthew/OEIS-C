/*
 * A011960  Number of ferrites M_2Y_n that repeat after 6n+10 layers.
 * https://oeis.org/A011960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/6⌋ + 1, except a(n) = ⌊n/6⌋ when n ≡ 1 (mod 6)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    seq_t base = (seq_t)(n / 6);
    return (r == 1) ? base : (base + 1);
}
