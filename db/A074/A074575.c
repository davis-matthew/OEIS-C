/*
 * A074575  a(n) = 5^n + 7^n + 9^n.
 * https://oeis.org/A074575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 7^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   power_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power_seq.h"

seq_t a(idx_t n)
{
    return power_seq((seq_t)5, n) + power_seq((seq_t)7, n) + power_seq((seq_t)9, n);
}
