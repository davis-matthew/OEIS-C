/*
 * A030139  a(n+1) = sum of digits of (a(n) + a(n-1)).
 * https://oeis.org/A030139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1; a(1)=4; a(n+1)=sum_of_digits(a(n)+a(n-1))
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[24];
    if (!initialized)
    {
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)4;
        for (idx_t i = 2; i < 24; ++i)
        {
            seq_t sum = seq[i-1] + seq[i-2];
            seq_t digit_sum = 0;
            seq_t x = sum;
            while (x > 0)
            {
                digit_sum += x % (seq_t)10;
                x /= (seq_t)10;
            }
            seq[i] = digit_sum;
        }
        initialized = true;
    }
    return seq[n % 24];
}
